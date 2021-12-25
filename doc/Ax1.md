# M1 Exploration - v 0.70

This remains a preliminary version.
Everything up to L1 cache discussion I am fairly confident of, having had some time to perform experiments, read the patents carefully, and edit the writing.
The L1 cache discussion and everything subsequent is a lot more tentative.
Poorly/ not yet edited, no experiments performed (or I’m unhappy with some of the experiments because I keep thinking of new ways to interpret the results so they’re not definitive).
Many of the patents I refer to I think I have the essential ideas correct, but that’s the result of a quick scan and analysis, not a thorough reading or a tracking down of all the related patents.

Even so, pretty soon the A15 and its companion chips (M2?)
will be out, and people will want to start exploring those.
Hopefully with this as vade mecum, they can spend more time getting to fundamental analysis and truly understanding what’s new, and less time re-inventing the wheel.

This is a CC0 1.0 Universal document meaning you can do what you like with it, you don’t need my permission.
It’s your choice as to whether, given that freedom, you behave like a decent human being or like a dick.
You want to pretend my words or investigations are yours go ahead -- but nothing stays secret for long on the internet.
If someone concludes they really want to translate this, go ahead, you don’t need my permission.

We’re all in this together, trying to understand.
However I’m exhausted and others probably also have good ideas.
I hope others can contribute, so we can continue the great work.
I will keep updating this and maybe publishing new updates every month or so.
Who knows when it will be done?
When I started I had no idea it would become such a deep dive, or that so much could be (somewhat reliably) established.

This document was written for myself, to remind me of, and to organize, my investigations into the M1.
These investigations took the form of experiments, and reading many Apple patents, all tied together by a reasonable knowledge of the academic literature.
The audience is anyone who is interested in technical details of the M1.
The level is somewhat choppy, but assume a substantially higher degree of knowledge than ye average internet opinionator on CPU’s.
I’ve included a huge number of references to papers and patents -- if you want to understand this stuff, read them.
Yes it takes works.
Yes, you have to do that work; no-one else can do it for you.

I expect the presentation will be to pretty much no-one’s tastes.
What can I say - skip over the parts that don’t appeal to you, whether that’s how an experiment was designed, how it was interpreted, a description of the literature surrounding a point, or a patent dump.
There is some repetition, in part because some material naturally fits into more than one place, and because reading the same thing in different contexts helps with understanding.

Obviously I’ve done my best to make this accurate.
Even so, there are probably multiple errors, whether of experimental design, implementation, analysis, my understanding of a patent, or anything else.
Technical corrections are welcomed.

## Experimental Setup

To run the experiments you will need a test setup.
I used the one created by Dougall Johnson here https://gist.github.com/dougallj/5bafb113492047c865c0c8cfbc930155, and https://gist.github.com/dougallj/c9976a52d592af24960ea7989cf652b1.

Dougall is the true hero of all this M1 investigation, doing the hard work of creating a useful test harness, especially all the low-level OS nonsense required to create JIT'able pages, set up the CPU counters, and so on, along with a python script to convert lines of ARMv8 assembly into machine code (required to make any interesting modifications to the tests).
Dougall also created the M1 instruction cycle counts web page at https://dougallj.github.io/applecpu/firestorm-int.html, and it's worth reading his investigations into the M1 at https://dougallj.wordpress.com/author/dougallj/.

Having said all that, much of my technique deviates substantially from both what Dougall did and what (Travis Downs, and, earlier, Henry Wong did).
My technique is probably less numerically precise than Henry Wong's technique, but I found it easier to modify and change, something essential for this sort of open-ended research where one has no idea quite what to expect.

Note something extremely important and not at all obvious.

In Dougall's code there is a fragment of code called add_prep() that zero's out as many of the integer or FP/SIMD registers as feasible.
This seems like an optional flourish that might be important for certain specialty measurements (like the size of the physical register file).
Not so!

What is important is not the zero'ing of these registers (any value will do), it is marking them as "being used by this app".
If this is not done then any code that reads these registers will run substantially slower than expected.
For example, while the throughput of basic integer ADD is 6-wide, if your code is reading a register like x5 (eg ADD x0, x5, x5) that has not been "claimed" it will run at something more like 4-wide, with constant weirdness and results that don't make sense.

I mention this because, of course, I hit this very issue, occasionally switching this off (in my version of the code), forgetting to switch it on, and then wanting to cry as nothing I did from that point on made any sense or matched my earlier results.

(Note that little of this will make sense, especially if you're not yet an OoO CPU expert, till you have read the entire report.)

What is actually going on here?!?
I have no idea, but much much later, after we understand many more features of the M1, we will revisit the issue.
My guess is that we are colliding with a security feature that is supposed to prevent access to "unauthorized" registers.

Recall the SPECTRE exploits some years ago in all their various forms; the common theme was a fear that, under the right circumstances, generally speculation beyond an inappropriate point, a piece of code could read or perhaps even "influence the value of" machine structures that were not appropriate to it.
Apple has a specific patented solution for this for the branch predictor; essentially every entry in the branch predictor is tagged with a value that incorporates at least some bits from every sort of indicator that might be violated by an exploit, so the tag includes an exception level (hypervisor, OS, user), a processID, even some high address bits (to catch JIT'd code spying on the rest of the process).
We will discuss this much later when we cover branches.

However this same scheme is very general, and is almost certainly being applied to registers.
The idea would be have a security tag for each mapping in the architectural to physical address mapping.
Each time the mapping is referenced, the tag is compared with the current security tag and, in the event of a mismatch, various things happen which presumably include the CPU
- providing some value (zero, or random) that is not the architected value (and the value in the physical register), meaning that an app cannot, eg, read the registers used by the OS across a system call
- noting the mismatch somewhere, incrementing some register
- providing, probably, some debug mode (maybe only available within Apple) that would force an exception at this point.

Normal code should never find itself in a position where it is reading a register that it, itself, did not previously write (at which point thee security tag was set).
It's only either malicious code, buggy code, or weird code (like ours, which cares only by timing ADDs, not the fact that the ADD is reading a random register value!)
that would ever read a register with a mismatching security tag.

So, put it bluntly, every time my code tries to read x5, the core will notice that x5 is currently "owned" by some other thread (probably the OS or an interrupt), and this incurs some substantial additional cost.
By overwriting every register at the start of our code, we "claim ownership" of that register and avoid this security violation and, in particular, its overhead.

We can probe this further.
What if we use something like MOV xn, xn, rather than the current MOV xn, #0, to force the overwrite?
That does not work!
Either that instructions is mapped to NOP earlier in the pipeline, or the zero-cycle rename stuff kicks in, without ever validating the thread ownership of the source register.
(My bet is on MOV xn, xn, being mapped to NOP.)

It's worth noting that I can't generate the same sort of disaster by not forcing ownership of the FP registers.
If the "loss of ownership" of the registers is occurring at an OS-call or interrupt boundary, and the OS/interrupt does not touch FP registers, this makes sense.

There is an alternative possibility, that we are seeing a mechanism for hardware context switching, to be discussed much later.
However the facts seem a better fit with the security violation explanation.

As an aside, at this point might I point out how much I absolutely LOATHE XCode.
Everyone associated with the Debug side of the product should be deeply ashamed of themselves.
It is beyond pathetic that this multigigabyte behemoth provides a worse debugging experience than freaking Macsbug from 1981, let alone Think C or Metrowerks.
In particular, the inability to IMMEDIATELY display a pointer as an array of that time is beyond incomprehensible.

If I never engage in any more of this work for further Apple SoCs, a desire never again to repeat the XCode experience will be a large part of the reason.

## Mathematica Setup (not relevant if you’re reading the PDF)

Need to use a conditional on "printing to PDF" to hide this!
This writeup was all done in Mathematica.
If you have access to Mathematica, you can download the companion notebook and look at the actual numbers, draw your own graphs from those numbers etc.
But most people don't have Mathematica, so for you I've printed the notebook to a PDF.

If you use Mathematica, we need a way to paste results data from the command line apps into Mathematica.
Easiest solution appears to be http://szhorvat.net/pelican/pasting-tabular-data-from-the-web.html

Note the "Show Input" button at the top of this notebook; toggle it if you want to see the (sometimes copious!)
input data for any particular graph.

The Mathematica code below adds that functionality to this notebook (not shown when "Show Input" is untoggled.

Also ctrl - clicking on a graph brings up a contextual menu, one of whose items, "Get Coordinates" is useful in getting a quick, reasonably accurate, feel for the coordinates of a point .

## Theory of a modern OoO machine

## Introduction

Around the 2000’s up to the 2010’s a number of nice articles were published giving overviews of how current OoO CPUs worked.
However 20, even 10, years is a long long time in CPU design (think what you would do if you had 2^5 or 2^10 x resources for a project, how differently you would proceed!
), and ideas that were state of the art back then are baseline today.

You should be able to read something like David Kanter' s Nehalem overview, (2008) https://www.realworldtech.
com/nehalem/ and understand all the terms introduced, know what a ROB is, know what instruction scheduling is, know why register renaming exists, and so on.
For a very simple overview of some aspects of a modern design, you should read (2008) https://carrv.github.io/2020/papers/CARRV2020_ paper_15_Zhao.pdf SonicBOOM: The 3rd Generation Berkeley Out-of-Order Machine.
Our goal is to move to a level substantially beyond that.

You will encounter a lot that is unfamiliar, but I will try at every stage to explain the reasons why things are done, or not done, as they are.

For obvious reasons, some of this article has to be speculation.
But it is informed speculation.
We have three types of sources available.
- There are academic papers, and I will frequently reference these, which explain that something can be done, at least one way of doing it, and how well it works. They don' t prove that anything is implemented, in the M1 or anywhere else, anywhere, but they do explain the details of a technique, and that it is feasible.
- There are Apple patents, which explain in detail the precise innovation that is to be patented, and that often, as part of their explanation, include other interesting details of the design. It is always possible to claim that a patent, and the details that it contains, tell us nothing about how Apple actually does things; that the patent was simply filed as a good idea that Apple may eventually want to use but not yet. That certainly has happened - Apple has a large collection of patents filed around an architecture called Macroscalar (2004) https://patents.google.com/patent/US8412914B2 Macroscalar processor architecture, based on very flexible indefinite length vectors, and which have not yet turned into a product. (It' s unclear whether SVE was in part inspired by Macroscalar; many of the ideas seem similar…) However when a patent is narrowly defined, makes clear sense, and fits in with everything else we know, it' s sheer stubbornness to insist that it does not "prove" anything; our goal here is understanding, not impossible standards of certainty that will never be attained until perhaps the relevant designers write their memoirs .

To put the patents in context, remember a few dates:
- Sept 2012 is the release of Swift/A6. 3-wide OoO, first (visible...) Apple core 1st gen
- Sept 2013 is the release of Swift/A7. 6-wide OoO, add 64-bit 2nd gen
- Sept 2017 is the release of A11. 8-wide OoO, drop 32-bit, clusters (P and E) as the basic unit 3rd gen
- Sept 2021 [reckless speculation!] 10-wide?, ARMv9?, virtual registers? 4th gen
- Sept 2025 [doubleplus speculation!] 12-wide? drop x86 support? 5th gen
- Finally there are code experiments. One can write carefully designed programs, measure their timing, perhaps augment that information with readings from Performance Monitor Counters, and try to figure out from the results what' s going on. This is the only way to establish quantitative information - but one has to be careful.

Ultimately all the program tells you is how long it took; it does not tell you why.
Benchmarks without understanding gives you Phoronix, but we subscribe to the Richard Hamming viewpoint: the goal of computing is insight, not numbers.

Much of the early information about the M1 is not so much incorrect as extremely incomplete, because people have been running these programs without a good model of the machine, interpreting it as much like a standard x86 machine.
Our goal here is to go beyond that, to explain how these programs work, what they measure, and how to interpret what they measure.

I would hardly expect you to be able to read the papers or patents referenced right now.
But hopefully, if you get to this end of this document and want to learn more, you' ll be in a position to work your way through them.
Be patient!
The first few times reading a paper or a patent is very difficult.
The trick is to accept that you don' t need to understand everything.
Read the parts that you understand, skip the parts that don' t interest you.
But take time to struggle through the parts that do interest you, but are unfamiliar - that' s where the value is!

You will find, after repeating the exercise five or ten times, you have begun to understand the structure of patents and papers, at which point you can be a lot faster, knowing what can be skipped over and immediately heading for the good stuff.
I tend to skim the diagrams, looking for those that represent the part I care about, then looking for the explanation of the diagram in the DETAILED DESCRIPTION OF EMBODIMENTS section (this is most easily done by search for one of the numbers that appears on the diagram).
Lawyers care especially about the Claims section, which is the legally binding part, but I find there tends to be nothing interesting there; Whereas the Detailed Description part tends to be fully of statements like "in one embodiment of", which is almost certainly going to be the way Apple actually do it in their implementation.
Sometimes what' s described in the patent may seem petty, or obvious, but in part becoming "one skilled in the art" is being able to look past the petty, obvious, parts to pick out the one gem, the one part that' s new or interesting.

### The Basic Speculative Superscalar OoO Machine

Consider the basic out of order superscalar machine, as of say around 2000.
Once we understand the general idea of this machine, we can consider all the many dimensions along which to improve it.

The CPU pipeline starts with a mechanism for deciding the address from which to load the next few instructions.
This is non-trivial!

Normal’ish code contains about a branch every six or so instructions.
The numbers vary depending on the type of code, but we can assume around a half of them are taken (meaning that the instruction pointer changes in some discontinuous way after the branch).
So we are talking a change in PC around every ten instructions or so.
If you’re trying to run at around 8 instructions per cycle, you need to be able to handle a new PC discontiguous with the previous run of instructions, every cycle.
Clearly you cannot wait for the last branch instruction executed to tell you what the new PC is.
Even under the best of conditions, that would result in a delay of maybe five or so pipeline stages between the fetch of a branch and when it is executed - five cycles while your fetch stage and most of your CPU is waiting, until it can jump to the next run of instructions.
Clearly unacceptable!

And so we use branch prediction.
The CPU guesses (based on past history) what the new PC will be at this point in execution (ie if the previous 64 times we encountered this branch, it jumped to address X, it’s a good bet that it will do the same thing this time).
For now let’s ignore the details of how branch predictors work beyond accepting that they continually inform fetch of a (generally very good guess) as the address of the next few instructions in the execution stream.

The consequence of this prediction is that almost every instruction on the machine is executed in a speculative state, meaning that the machine needs to hold every result generated (including all stores) in temporary storage until the point at which it’s clear that all the branches that affect a given instruction were correct, and so that instruction can Commit its state (that is, convert it from something temporary into something permanent).

We also perform instructions out of order.
It’s a somewhat surprising fact of real world programs that there’s a lot of independence between successive instructions.
This takes two forms.
There is "immediate" parallelism, where successive stages of a chain of execution each require two or three independent operations.

If we bundle those together as a single "macroinstruction", we then tend to have chains of sequentially dependent macroinstructions, each about two to three instructions wide.
Your intuition might be that this means most code can only run about two to three wide, but that’s not the case!

What most code looks like is that it consists of short chains of sequentially dependent macroinstructions (say 5 to 7 macroinstructions, 10 to 20 instructions long in total) which store their result to memory or a register, and that memory or register is not accessed until many (hundreds) of cycles later.
This means that while each sequentially dependent macroinstruction has to execute one after the other, you can execute many of the chains in parallel...

That sounds good but you need a variety of machinery to track which instructions are independent of previous instructions, and to track the program order of instructions so that as branches are resolved as correct, you know which of the instructions in program order now resolve as correct.

(This fact is why so many people’s intuition about the value of superscalarity is so flawed.
Most people hone their assembly optimization skills on long stretches of sequentially dependent instructions; but such code is actually unrepresentative of most of what runs on a CPU.

This fact is also why OoO superscalarity works so well, whereas most attempts to create static wide machines have been problematic.
All the pieces -- out of order, prediction, and superscalarity -- work synergistically.
In particular most of these chains that are running in parallel come from different basic blocks [ie are separated by some sort of if() statement that the compiler can’t see past] and so are impossible to aggregate statically.)

So the basic machine fetches instruction in a guessed instruction order, allocates resources to each instruction in order, throws the instructions into a large pool from which they Execute as soon as they can (ie once their Dependencies are satisfied), and then Retires the instructions in complete program order.

Retirement is the point at which all the guesses along the way are tested for correctness (and if they fail, we flush everything after the wrong guess and start again).
Retire (because it happens in order) also undoes all the confusion caused by the OoO execution.

So let's think about the consequences of all this.
What sort of state needs to be held as tentative until it can be Committed?

One obvious set of state is stores.

There are less obvious issues surrounding loads.

There are possible exceptions that were raised (eg by loads or stores that had invalid addresses).

And there are register values that need to be restored to whatever the programmer view of the registers was at the point of restoration.

(Even less obvious is the state that is used to inform predictors.
You can update your branch predictors, or your prefetchers, as soon as the relevant instructions are executed.
But you may be updating them with flawed data...
It turns out, another non-intuitive result, that on the data side, eg for data prefetch, this is mostly not a big problem, whereas on the instruction side, if you want quality accuracy for both your branch predictors and your instruction prefetchers, you need to ensure that they are not polluted by incorrectly speculated paths.
[You also need to ensure that they are not polluted by interrupts, which throw in behavior that doesn’t actually represent the flow of control you are trying to model.])

So we need a variety of structures to keep track of all this.
Traditionally the largest of these structures is known as the ROB.
This stands for Reorder Buffer which is not an especially helpful name.
A better way to think of it is as Retirement Buffer, and to think of retirement as the point where two tasks are performed
- every instruction is given its last chance to either Commit its results to programer state (ie we agree that the instruction is not speculative at this point, and has raised no problematic issues like an exception)
- all resources that were allocated to the instruction can be returned to the machine for reuse. (We can ignore the second point for now, but will return to it in time.)

So the basic flow of instructions (and remember, under ideal circumstances, each of these stages is dealing with around 8 instructions during the same cycle, and all stages are happening in parallel on a different 8 instructions!)
is:
- Fetch (all the branch prediction machinery)
- Decode
- Map
- Rename
- Coarse Scheduling
- Fine-grained Scheduling
- Execution
- Retirement
- Commit

What do each of these do?

**Decode** transforms instructions from their representation in memory (ie 32 bits) into something that’s more convenient for the machine.
Of interest to us, at this stage pairs of instructions may be fused, or meaningless instructions (mainly NOP) can be thrown away as irrelevant to the rest of the pipeline.
NOP might seem a special case, not worth special treatment.
Why bother with an instruction that does nothing, and why bother to treat it efficiently?

In the first place, much of the low-level machinery of modern operating systems is based on dynamic linkers and position independent code.
This code is created in multiple pieces (separate compilation) and joined together by a linker.
This joining process (the details of creating code that is position independent and can act as a library that can be called simultaneously by multiple programs) requires that all the calls in the code (app or library) that look like they might cross from one piece of code to another need to have a particular structure that might consist of two or three specific instructions.
But when the linker actually stitches the code together, many of the calls that seemed like they might need to be "international" are in fact only local.
Hence the two or three instruction slots that were reserved for an "international call" to a separately maintained piece of code, can be filled in with a local call (single instruction) and one or two NOPs.

Thus real world code contains a surprising number of NOPs, so why not make them as cheap as possible?

ARM also defines a family of HINT instructions (basically NOPs with different bits set in the instruction) might act as various hints for prefetching, branch prediction or other such control.
A machine may understand a particular hint, in which case it will be treated as a real instruction, or it may not understand this particular hint, in which case it will be treated as a NOP and just ignored.

**Map** handles register renaming.
(Yes the naming seems wrong, be patient.)
Recall that part of the machinery of speculative OoO is that the logical registers of the machine (the programmer visible registers, let’s say 32 for ARM integer registers) are mapped onto a much larger pool (hundreds) of physical registers.

The idea is that at any particular time in the CPU, there is a map saying that logical register `rN` is mapped to physical register `pM`.
So consider an instruction like

`ADD r2, r1, r0`

This instruction has two input registers (`r0` and `r1`) so we need to consult the mapping table to figure out that r0 is currently mapped to physical register `p7`, and `r1` is mapped to `p45`.

The add also takes a destination register, `r2`, so we need to create a new mapping for `r2` (a new mapping is created every time a register is overwritten).
This "single-write" rule means that the physical register can holds temporary state as long as the instruction is speculative; no other instruction is allowed to reuse a physical register until we can be absolutely certain that the temporary result it stores won’t ever be needed again.

This concept of renaming registers should be familiar, and you should take some time to think about exactly how it operates, how the mapping table would be updated, when it is safe to reuse a register, and so on.
But the most difficult part of the problem is one you probably didn’t think of!

Remember, the machine may have to remap (source registers, plus allocation of new physical registers for all destination registers) up to eight instructions in a cycle.
The tricky part of this is that often the same registers are used in many of these instructions.
For a trivial case consider just the two instructions

```
ADD rA, rB, rC
MUL rD, rA, rF
```

The `rA` physical register must be allocated for the Add, before the mul is handled because the physical register looked up in the mapping table for rA (and rF) must reflect the mappings after the add instruction.

So you can’t just remap all eight instructions independently!
You have to figure out the successive name dependencies between all the instructions and treat that appropriately.
That’s the most difficult job of Map, and why it has a separate pipeline stage.
It’s an interesting fact that Apple is very clear, in multiple patents, that they use this separate Map stage, whereas most other companies do not mention such a stage.
Being willing to move the most difficult part of the job to a separate stage may be part of why Apple has been able to maintain such spectacular CPU widths?

**Rename** is the traditional name given to a stage that should really be called Allocate.
This is the stage where each instruction is given the resources it requires to perform its job as part of a speculative OoO machine.

What sort of resources need to be allocated?

The most basic is a slot in the ROB.
The ROB is a queue of instruction in program order (this order is speculative, as best can be guessed by the branch predictor, but assuming that’s correct, instruction are stored in the ROB in program order with no OoO weirdness).
Each cycle the instruction that have just arrived in Rename are allocated a slot at the end of the ROB, while (as a separate stage) the instructions at the head of the ROB are tested to see if they have completed, and completed correctly.

If the instructions at the head of the ROB have completed correctly their resources are returned to the system and the head of the ROB queue moves down a few slots.
