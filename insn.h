/* https://nptel.ac.in/content/storage2/courses/106108100/pdf/Teacher_Slides/mod2/M2L1.pdf */
/* https://en.wikipedia.org/wiki/X86_instruction_listings */
#define AAA_ "aaa \n\t"
void AAA(void); //original
#define AAD_ "aad \n\t"
#define AADi(IMM) "aad " #IMM "\n\t"
void AAD(void); //original
#define AAM_ "aad \n\t"
#define AAMi(IMM) "aad " #IMM "\n\t"
void AAM(void); //original
#define AAS_ "aas \n\t"
void AAS(void); //original
#define ADC_(OPN1, OPN2) "adc " #OPN1 "," #OPN2 "\n\t"
void ADC(void); //original
#define ADD_(OPN1, OPN2) "add " #OPN1 "," #OPN2 "\n\t"
void ADD(void); //original
#define ADDPD_(OPN, XMM) "addpd " #OPN ",%" #XMM "\n\t"
void ADDPD(void); //SSE2 packed arithmetic instructions
#define ADDPS_(OPN, XMM) "addps " #OPN ",%" #XMM "\n\t"
void ADDPS(void); //SSE2 packed arithmetic instructions
#define ADDSD_(OPN, XMM) "addsd " #OPN ",%" #XMM "\n\t"
void ADDSD(void); //SSE2 packed arithmetic instructions
#define ADDSS_(OPN, XMM) "addss " #OPN ",%" #XMM "\n\t"
void ADDSS(void); //SSE2 packed arithmetic instructions
#define ADDSUBPD_(OPN, XMM) "addsubpd " #OPN ",%" #XMM "\n\t"
void ADDSUBPD(void); //SSE3 SIMD floating-point instructions
#define ADDSUBPS_(OPN, XMM) "addsubps " #OPN ",%" #XMM "\n\t"
void ADDSUBPS(void); //SSE3 SIMD floating-point instructions
#define AND_(OPN1, OPN2) "and " #OPN1 "," #OPN2 "\n\t"
void AND(void); //original
#define ANDPD_(OPN, XMM) "andpd " #OPN ",%" #XMM "\n\t"
void ANDPD(void); //SSE2 logical instructions
#define ANDPS_(OPN, XMM) "andps " #OPN ",%" #XMM "\n\t"
void ANDPS(void); //SSE instructions
#define ANDNPD_(OPN, XMM) "andnpd " #OPN ",%" #XMM "\n\t"
void ANDNPD(void); //SSE2 logical instructions
#define ANDNPS_(OPN, XMM) "andnps " #OPN ",%" #XMM "\n\t"
void ANDNPS(void); //SSE instructions
#define ARPL_(OPN, REG) "arpl " #OPN ",%" #REG "\n\t"
void ARPL(void); //Added with 80286
// what about prefix
#define BLENDPD_(IMM, OPN1, XMM) "blendpd " #IMM "," #OPN1 ",%" #XMM "\n\t"
void BLENDPD(void); //SSE4.1 SIMD floating-point instructions
#define BLENDPS_(IMM, OPN1, XMM) "blendps " #IMM "," #OPN1 ",%" #XMM "\n\t"
void BLENDPS(void); //SSE4.1 SIMD floating-point instructions
#define BLENDVPD_(XMM1, OPN1, XMM2) "blendvpd %" #XMM1 "," #OPN1 ",%" #XMM2 "\n\t"
void BLENDVPD(void); //SSE4.1 SIMD floating-point instructions
#define BLENDVPS_(XMM1, OPN1, XMM2) "blendvps %" #XMM1 "," #OPN1 ",%" #XMM2 "\n\t"
void BLENDVPS(void); //SSE4.1 SIMD floating-point instructions
#define BOUND_(OPN, REG) "bound " #OPN ",%" #REG "\n\t"
void BOUND(void); //Added with 80186/80188
void BSF(void); //Added with 80386
void BSR(void); //Added with 80386
void BSWAP(void); //Added with 80486
void BT(void); //Added with 80386
void BTC(void);
void BTR(void);
void BTS(void);
void BZHI(void);
void CALL(void);
void CBW(void);
void CWDE(void);
void CDQE(void);
void CLC(void);
void CLD(void);
void CLFLUSH(void);
void CLFLUSHOPT(void);
void CLI(void);
void CLRSSBSY(void);
void CLWB(void);
void CMC(void);
//A AE B BE C E G GE L LE NA NAE NB NBE NC NE NG NGE NL NLE NO NP NS NZ O P PE PO S Z
void CMOVcc(void);
void CMP(void);
void CMPPD(void);
void CMPPS(void);
void CMPS(void);
void CMPSB(void);
void CMPSW(void);
void CMPSD(void);
void CMPSQ(void);
void CMPSD(void);
void CMPSS(void);
void CMPXCHG(void);
void CMPXCHG8B(void);
void CMPXCHG16B(void);
void COMISD(void);
void COMISS(void);
void CPUID(void);
void CRC32(void);
void CVTDQ2PD(void);
void CVTDQ2PS(void);
void CVTPD2DQ(void);
void CVTPD2PI(void);
void CVTPD2PS(void);
void CVTPI2PD(void);
void CVTPI2PS(void);
void CVTPS2DQ(void);
void CVTPS2PD(void);
void CVTPS2PI(void);
void CVTSD2SI(void);
void CVTSD2SS(void);
void CVTSI2SD(void);
void CVTSI2SS(void);
void CVTSS2SD(void);
void CVTSS2SI(void);
void CVTTPD2DQ(void);
void CVTTPD2PI(void);
void CVTTPS2DQ(void);
void CVTTPS2PI(void);
void CVTTSD2SI(void);
void CVTTSS2SI(void);
void CWD(void);
void CDQ(void);
void CQO(void);
void DAA(void);
void DAS(void);
#define DIV_(OPN1) "div " #OPN1 "\n\t"
void DEC(void);
void DIV(void);
void DIVPS(void);
void DIVSS(void);
void DPPD(void);
void DPPS(void);
void EMMS(void);
void ENTER(void);
void EXTRACTPS(void);
void F2XM1(void);
void FABS(void);
void FADD(void);
void FADDP(void);
void FIADD(void);
void FBLD(void);
void FBSTP(void);
void FCHS(void);
void FCLEX(void);
void FNCLEX(void);
//B E BE U NB NE NBE NU
void FCMOVcc(void);
void FCOM(void);
void FCOMP(void);
void FCOMPP(void);
void FCOMI(void);
void FCOMIP(void);
void FUCOMI(void);
void FUCOMIP(void);
void FCOS(void);
void FDECSTP(void);
void FDIV(void);
void FDIVP(void);
void FIDIV(void);
void FDIVR(void);
void FDIVRP(void);
void FIDIVR(void);
void FFREE(void);
void FICOM(void);
void FICOMP(void);
void FILD(void);
void FINCSTP(void);
void FINIT(void);
void FNINIT(void);
void FIST(void);
void FISTP(void);
void FISTTP(void);
void FLD(void);
void FLD1(void);
void FLDL2T(void);
void FLDL2E(void);
void FLDPI(void);
void FLDLG2(void);
void FLDLN2(void);
void FLDZ(void);
void FLDCW(void);
void FLDENV(void);
void FMUL(void);
void FMULP(void);
void FIMUL(void);
void FNOP(void);
void FPATAN(void);
void FPREM(void);
void FPREM1(void);
void FPTAN(void);
void FRNDINT(void);
void FRSTOR(void);
void FSAVE(void);
void FNSAVE(void);
void FSCALE(void);
void FSIN(void);
void FSINCOS(void);
void FSQRT(void);
void FST(void);
void FSTP(void);
void FSTCW(void);
void FNSTCW(void);
void FSTENV(void);
void FNSTENV(void);
void FSTSW(void);
void FNSTSW(void);
void FSUB(void);
void FSUBP(void);
void FISUB(void);
void FSUBR(void);
void FSUBRP(void);
void FISUBR(void);
void FTST(void);
void FUCOM(void);
void FUCOMP(void);
void FUCOMPP(void);
void FXAM(void);
void FXCH(void);
void FXRSTOR(void);
void FXSAVE(void);
void FXTRACT(void);
void FYL2X(void);
void FYL2XP1(void);
void HADDPD(void);
void HADDPS(void);
void HSUBPD(void);
void HSUBPS(void);
void IDIV(void);
void IMUL(void);
void IN(void);
void INC(void);
void INCSSPD(void);
void INCSSPQ(void);
void INS(void);
void INSB(void);
void INSW(void);
void INSD(void);
void INSERTPS(void);
void INT_imm8(void);
void INTO(void);
void INT3(void);
void INT1(void);
//A AE B BE C E G GE L LE NA NAE NB NBE NC NE NG NGE NL NLE NO NP NS NZ O P PE PO S Z
void Jcc(void);
void JMP(void);
void LAHF(void);
void LAR(void);
void LDDQU(void);
void LDMXCSR(void);
void LDS(void);
void LES(void);
void LFS(void);
void LGS(void);
void LSS(void);
void LEA(void);
void LEAVE(void);
void LFENCE(void);
/* LOCK???Assert LOCK# Signal Prefix--page 1186 */
void LODS(void);
void LODSB(void);
void LODSW(void);
void LODSD(void);
void LODSQ(void);
void LOOP(void);
// E NE
void LOOPcc(void);
void LSL(void);
void LZCNT(void);
void MASKMOVDQU(void);
void MASKMOVQ(void);
void MAXPD(void);
void MAXPS(void);
void MAXSD(void);
void MAXSS(void);
void MFENCE(void);
void MINPD(void);
void MINPS(void);
void MINSD(void);
void MINSS(void);
#define MOV_(OPN1, OPN2) "mov " #OPN1 "," #OPN2 " \n\t"
void MOV(void);
#define MOVAPD_(OPN1, OPN2) "movapd " #OPN1 "," #OPN2 "\n\t"
void MOVAPD(void);
#define MOVAPS_(OPN1, OPN2) "movaps " #OPN1 "," #OPN2 "\n\t"
void MOVAPS(void);
void MOVBE(void);
void MOVD(void);
void MOVQ(void);
void MOVDDUP(void);
void MOVDQA(void);
void MOVDQU(void);
void MOVDQ2Q(void);
void MOVHLPS(void);
void MOVHPD(void);
void MOVHPS(void);
void MOVLHPS(void);
void MOVLPD(void);
void MOVLPS(void);
void MOVMSKPD(void);
void MOVMSKPS(void);
void MOVNTDQA(void);
void MOVNTDQ(void);
void MOVNTI(void);
void MOVNTPD(void);
void MOVNTPS(void);
void MOVNTQ(void);
void MOVQ2(void);
void MOVQ2DQ(void);
void MOVS(void);
void MOVSB(void);
void MOVSW(void);
void MOVSD(void);
void MOVSQ(void);
// ambiguous ???
void MOVSD2 (void);
void MOVSHDUP(void);
void MOVSLDUP(void);
void MOVSS(void);
void MOVSX(void);
void MOVSXD(void);
void MOVUPD(void);
void MOVUPS(void);
void MOVZX(void);
void MPSADBW(void);
#define MUL_(OPN1) "mul " #OPN1 "\n\t"
void MUL(void);
void MULPD(void);
void MULPS(void);
void MULSD(void);
void MULSS(void);
void MULX(void);
void NEG(void);
void NOP(void);
void NOT(void);
void OR(void);
void ORPD(void);
void ORPS(void);
void OUT(void);
void OUTS(void);
void OUTSB(void);
void OUTSW(void);
void OUTSD(void);
void PABSB(void);
void PABSW(void);
void PABSD(void);
void PABSQ(void);
void PACKSSWB(void);
void PACKSSDW(void);
void PACKUSDW(void);
void PACKUSWB(void);
void PADDB(void);
void PADDW(void);
void PADDD(void);
void PADDQ(void);
void PADDSB(void);
void PADDSW(void);
void PADDUSB(void);
void PADDUSW(void);
void PALIGNR(void);
void PAND(void);
void PANDN(void);
void PAUSE(void);
void PAVGB(void);
void PAVGW(void);
void PBLENDVB(void);
void PBLENDW(void);
void PCLMULQDQ(void);
void PCMPEQB(void);
void PCMPEQW(void);
void PCMPEQD(void);
void PCMPEQQ(void);
void PCMPESTRI(void);
void PCMPESTRM(void);
void PCMPGTB(void);
void PCMPGTW(void);
void PCMPGTD(void);
void PCMPGTQ(void);
void PCMPISTRI(void);
void PCMPISTRM(void);
void PDEP(void);
void PEXT(void);
void PEXTRB(void);
void PEXTRD(void);
void PEXTRQ(void);
void PEXTRW(void);
void PHADDW(void);
void PHADDD(void);
void PHADDSW(void);
void PHMINPOSUW(void);
void PHSUBW(void);
void PHSUBD(void);
void PHSUBSW(void);
void PINSRB(void);
void PINSRD(void);
void PINSRQ(void);
void PINSRW(void);
void PMADDUBSW(void);
void PMADDWD(void);
void PMAXSB(void);
void PMAXSW(void);
void PMAXSD(void);
void PMAXUB(void);
void PMAXUW(void);
void PMAXUD(void);
void PMINSB(void);
void PMINSW(void);
void PMINSD(void);
void PMINUB(void);
void PMINUW(void);
void PMINUD(void);
void PMOVMSKB(void);
void PMOVSX(void);
void PMOVZX(void);
void PMULDQ(void);
void PMULHRSW(void);
void PMULHUW(void);
void PMULHW(void);
void PMULLD(void);
void PMULLW(void);
void PMULUDQ(void);
void POP(void);
void POPA(void);
void POPAD(void);
void POPCNT(void);
void POPF(void);
void POPFD(void);
void POPFQ(void);
void POR(void);
void PREFETCHT0(void);
void PREFETCHT1(void);
void PREFETCHT2(void);
void PREFETCHNTA(void);
void PSADBW(void);
void PSHUFB(void);
void PSHUFD(void);
void PSHUFHW(void);
void PSHUFLW(void);
void PSHUFW(void);
void PSIGNB(void);
void PSIGNW(void);
void PSIGND(void);
void PSLLDQ(void);
void PSLLW(void);
void PSLLD(void);
void PSLLQ(void);
void PSRAW(void);
void PSRAD(void);
void PSRLDQ(void);
void PSRLW(void);
void PSRLD(void);
void PSRLQ(void);
void PSUBB(void);
void PSUBW(void);
void PSUBD(void);
void PSUBQ(void);
void PSUBSB(void);
void PSUBSW(void);
void PSUBUSB(void);
void PSUBUSW(void);
void PTEST(void);
void PUNPCKHBW(void);
void PUNPCKHWD(void);
void PUNPCKHDQ(void);
void PUNPCKHQDQ(void);
void PUNPCKLBW(void);
void PUNPCKLWD(void);
void PUNPCKLDQ(void);
void PUNPCKLQDQ(void);
void PUSH(void);
void PUSHA(void);
void PUSHAD(void);
void PUSHF(void);
void PUSHFD(void);
void PUSHFQ(void);
void PXOR(void);
void RCPPS(void);
void RCPSS(void);
void RDRAND(void);
void RDSEED(void);
void RDSSPD(void);
void RDSSPQ(void);
void RDTSC(void);
void RDTSCP(void);
/* REP/REPE/REPZ/REPNE/REPNZ???Repeat String Operation Prefix -- see page 1758 */
void RET(void);
void RORX(void);
void ROUNDPD(void);
void ROUNDPS(void);
void ROUNDSD(void);
void ROUNDSS(void);
void RSM(void);
void RSQRTPS(void);
void RSQRTSS(void);
void RSTORSSP(void);
void SAHF(void);
void SAL(void);
void SAR(void);
void SHL(void);
void SHR(void);
void SARX(void);
void SHLX(void);
void SHRX(void);
void SAVEPREVSSP(void);
void SBB(void);
void SCAS(void);
void SCASB(void);
void SCASW(void);
void SCASD(void);
// A AW B BE C E G GE L LE NA NAE NB NBE NC NE NG NGE NL NLE NO NP NS NZ O P PE PO S Z
void SETcc(void);
void SFENCE(void);
void SHLD(void);
void SHRD(void);
void SHUFPD(void);
void SHUFPS(void);
void SLDT(void);
void SQRTPD(void);
void SQRTPS(void);
void SQRTSD(void);
void SQRTSS(void);
void STAC(void);
void STC(void);
void STD(void);
void STI(void);
void STMXCSR(void);
void STOS(void);
void STOSB(void);
void STOSW(void);
void STOSD(void);
void STR(void);
#define SUB_(OPN1, OPN2) "sub " #OPN1 "," #OPN2 " \n\t"
void SUB(void);
void SUBPD(void);
void SUBPS(void);
void SUBSD(void);
void SUBSS(void);
void SYSENTER(void);
void TEST(void);
void TZCNT(void);
void UCOMISD(void);
void UCOMISS(void);
void UD0(void);
void UD1(void);
void UD2(void);
void UNPCKHPD(void);
void UNPCKHPS(void);
void UNPCKLPD(void);
void UNPCKLPS(void);
void VERR(void);
void VERW(void);
void WAIT(void);
void FWAIT(void);
void XADD(void);
void XCHG(void);
void XGETBV(void);
void XLAT(void);
void XLATB(void);
void XOR(void);
void XORPD(void);
void XORPS(void);
