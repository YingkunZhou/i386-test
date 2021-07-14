#include "insn.h"

float as[4] __attribute__ ((aligned (16))) = {2.0f, 4.0f, 6.0f, 8.0f};
float bs[4] __attribute__ ((aligned (16))) = {1.0f, 3.0f, 5.0f, 7.0f};

double ad[2] __attribute__ ((aligned (16))) = {2.0, 4.0};
double bd[2] __attribute__ ((aligned (16))) = {1.0, 3.0};

/* http://www.c-jump.com/CIS77/CIS77syllabus.htm */
void AAA(void)
{
  __asm__ __volatile__(
      MOV_($0, %%ah)
      MOV_($6, %%al)
      ADD_($5, %%al)
      AAA_
      :::"ax");
}

void AAD(void)
{
  __asm__ __volatile__(
      MOV_($0x0205, %%ax)
      AAD_
      :::"ax");
  __asm__ __volatile__(
      MOV_($0x0607, %%ax)
      MOV_($9, %%ch)
      AAD_
      DIV_(%%ch)
      :::"ch","ax");
}

void AAM(void)
{
  __asm__ __volatile__(
      MOV_($5, %%al)
      MOV_($7, %%al)
      MUL_(%%bl)
      AAM_
      :::"bl","al");
}

void AAS(void)
{
  __asm__ __volatile__(
      MOV_($0x0901, %%ax)
      SUB_($9, %%al)
      "aas \n\t"
      :::"ax");
  __asm__ __volatile__(
      MOV_($0x39, %%al)
      MOV_($0x35, %%bl)
      SUB_(%%bl, %%al)
      AAS_
      :::"bl","al");
  /* ;AL=00000100 =BCD  04;CF = 0 NO Borrow required */
  __asm__ __volatile__(
      MOV_($0x35, %%al)
      MOV_($0x39, %%bl)
      SUB_(%%bl, %%al)
      AAS_
      :::"bl","al");
  /* ;AL=00000100 =BCD  04;CF = 1 NO Borrow required */
}

void ADC(void)
{
}

/* https://github.com/weidai11/cryptopp/issues/463 */
/* https://www.intel.com/content/dam/www/public/us/en/documents/white-papers/ia-large-integer-arithmetic-paper.pdf */
void ADCX(void)
{
}

/* ADDPD xmm1, xmm2/m128
 *   Destination[0..63]=Destination[0..63]+Source[0..63]
 *   Destination[64..127]=Destination[64..127]+Source[64..127]
 */
void ADDPD(void)
{
  __asm__ __volatile__(
      MOVAPD_(ad, %%xmm0)
      ADDPD_(bd, %xmm0)
      MOVAPD_(%%xmm0, ad)
      :::"xmm0");
}

/* ADDPS xmm1, xmm2/m128
 *   Destination[0..31]=Destination[0..31]+Source[0..31]
 *   Destination[32..63]=Destination[32..63]+Source[32..63]
 *   Destination[64..95]=Destination[64..95]+Source[64..95]
 *   Destination[96..127]=Destination[96..127]+Source[127-96]
 */
void ADDPS(void)
{
  __asm__ __volatile__(
      MOVAPS_(as, %%xmm0)
      ADDPS_(bs, %xmm0)
      MOVAPS_(%%xmm0, as)
      :::"xmm0");
}

/* ADDSD xmm1, xmm2/m64
 *   Destination[0..63]=Destination[0..63]+Source[0..63]
 *   Destination[64..127] remains unchanged
 */
void ADDSD(void)
{
  __asm__ __volatile__(
      MOVAPD_(ad, %%xmm0)
      ADDSD_(bd, %xmm0)
      MOVAPD_(%%xmm0, ad)
      :::"xmm0");
}

/* ADDSS xmm1, xmm2/m32
 *   Destination[0..31]=Destination[0..31]+Source[0..31]
 *   Destination[32..127]remain unchanged
 */
void ADDSS(void)
{
  __asm__ __volatile__(
      MOVAPS_(as, %%xmm0)
      ADDSS_(bs, %xmm0)
      MOVAPS_(%%xmm0, as)
      :::"xmm0");
}

void ADDSUBPD(void)
{
}

void ADDSUBPS(void)
{
}

void ADOX(void)
{
}

void AND(void)
{
}

void ANDN(void)
{
}

void ANDPD(void)
{
}

void ANDPS(void)
{
}

void ANDNPD(void)
{
}

void ANDNPS(void)
{
}

/* The ARPL instruction is provided for use by operating-system procedures (however, it can also be used by applica-
 * tions). It is generally used to adjust the RPL of a segment selector that has been passed to the operating system by
 * an application program to match the privilege level of the application program.
 */
void ARPL(void)
{
}

void BEXTR(void)
{
}

void BLENDPD(void)
{
}

void BLENDPS(void)
{
}

void BLENDVPD(void)
{
}

void BLENDVPS(void)
{
}

void BLSI(void)
{
}

void BLSMSK(void)
{
}

void BLSR(void)
{
}

void BNDCL(void)
{
}

void BNDCU(void)
{
}

void BNDCN(void)
{
}

void BNDLDX(void)
{
}

void BNDMK(void)
{
}

void BNDMOV(void)
{
}

void BNDSTX(void)
{
}

void BOUND(void)
{
}

void BSF(void)
{
}

void BSR(void)
{
}

void BSWAP(void)
{
}

void BT(void)
{
}

void BTC(void)
{
}

void BTR(void)
{
}

void BTS(void)
{
}

void BZHI(void)
{
}

void CALL(void)
{
}

void CBW(void)
{
}

void CWDE(void)
{
}

void CDQE(void)
{
}

void CLC(void)
{
}

void CLD(void)
{
}

void CLFLUSH(void)
{
}

void CLFLUSHOPT(void)
{
}

void CLI(void)
{
}

void CLRSSBSY(void)
{
}

void CLWB(void)
{
}

void CMC(void)
{
}

void CMOVcc(void)
{
}

void CMP(void)
{
}

void CMPPD(void)
{
}

void CMPPS(void)
{
}

void CMPS(void)
{
}

void CMPSB(void)
{
}

void CMPSW(void)
{
}

void CMPSD(void)
{
}

void CMPSQ(void)
{
}

void CMPSS(void)
{
}

void CMPXCHG(void)
{
}

void CMPXCHG8B(void)
{
}

void CMPXCHG16B(void)
{
}

void COMISD(void)
{
}

void COMISS(void)
{
}

void CPUID(void)
{
}

void CRC32(void)
{
}

void CVTDQ2PD(void)
{
}

void CVTDQ2PS(void)
{
}

void CVTPD2DQ(void)
{
}

void CVTPD2PI(void)
{
}

void CVTPD2PS(void)
{
}

void CVTPI2PD(void)
{
}

void CVTPI2PS(void)
{
}

void CVTPS2DQ(void)
{
}

void CVTPS2PD(void)
{
}

void CVTPS2PI(void)
{
}

void CVTSD2SI(void)
{
}

void CVTSD2SS(void)
{
}

void CVTSI2SD(void)
{
}

void CVTSI2SS(void)
{
}

void CVTSS2SD(void)
{
}

void CVTSS2SI(void)
{
}

void CVTTPD2DQ(void)
{
}

void CVTTPD2PI(void)
{
}

void CVTTPS2DQ(void)
{
}

void CVTTPS2PI(void)
{
}

void CVTTSD2SI(void)
{
}

void CVTTSS2SI(void)
{
}

void CWD(void)
{
}

void CDQ(void)
{
}

void CQO(void)
{
}

void DAA(void)
{
}

void DAS(void)
{
}

void DEC(void)
{
}

void DIV(void)
{
}

void DIVPS(void)
{
}

void DIVSS(void)
{
}

void DPPD(void)
{
}

void DPPS(void)
{
}

void EMMS(void)
{
}

void ENTER(void)
{
}

void EXTRACTPS(void)
{
}

void F2XM1(void)
{
}

void FABS(void)
{
}

void FADD(void)
{
}

void FADDP(void)
{
}

void FIADD(void)
{
}

void FBLD(void)
{
}

void FBSTP(void)
{
}

void FCHS(void)
{
}

void FCLEX(void)
{
}

void FNCLEX(void)
{
}

void FCMOVcc(void)
{
}

void FCOM(void)
{
}

void FCOMP(void)
{
}

void FCOMPP(void)
{
}

void FCOMI(void)
{
}

void FCOMIP(void)
{
}

void FUCOMI(void)
{
}

void FUCOMIP(void)
{
}

void FCOS(void)
{
}

void FDECSTP(void)
{
}

void FDIV(void)
{
}

void FDIVP(void)
{
}

void FIDIV(void)
{
}

void FDIVR(void)
{
}

void FDIVRP(void)
{
}

void FIDIVR(void)
{
}

void FFREE(void)
{
}

void FICOM(void)
{
}

void FICOMP(void)
{
}

void FILD(void)
{
}

void FINCSTP(void)
{
}

void FINIT(void)
{
}

void FNINIT(void)
{
}

void FIST(void)
{
}

void FISTP(void)
{
}

void FISTTP(void)
{
}

void FLD(void)
{
}

void FLD1(void)
{
}

void FLDL2T(void)
{
}

void FLDL2E(void)
{
}

void FLDPI(void)
{
}

void FLDLG2(void)
{
}

void FLDLN2(void)
{
}

void FLDZ(void)
{
}

void FLDCW(void)
{
}

void FLDENV(void)
{
}

void FMUL(void)
{
}

void FMULP(void)
{
}

void FIMUL(void)
{
}

void FNOP(void)
{
}

void FPATAN(void)
{
}

void FPREM(void)
{
}

void FPREM1(void)
{
}

void FPTAN(void)
{
}

void FRNDINT(void)
{
}

void FRSTOR(void)
{
}

void FSAVE(void)
{
}

void FNSAVE(void)
{
}

void FSCALE(void)
{
}

void FSIN(void)
{
}

void FSINCOS(void)
{
}

void FSQRT(void)
{
}

void FST(void)
{
}

void FSTP(void)
{
}

void FSTCW(void)
{
}

void FNSTCW(void)
{
}

void FSTENV(void)
{
}

void FNSTENV(void)
{
}

void FSTSW(void)
{
}

void FNSTSW(void)
{
}

void FSUB(void)
{
}

void FSUBP(void)
{
}

void FISUB(void)
{
}

void FSUBR(void)
{
}

void FSUBRP(void)
{
}

void FISUBR(void)
{
}

void FTST(void)
{
}

void FUCOM(void)
{
}

void FUCOMP(void)
{
}

void FUCOMPP(void)
{
}

void FXAM(void)
{
}

void FXCH(void)
{
}

void FXRSTOR(void)
{
}

void FXSAVE(void)
{
}

void FXTRACT(void)
{
}

void FYL2X(void)
{
}

void FYL2XP1(void)
{
}

void HADDPD(void)
{
}

void HADDPS(void)
{
}

void HSUBPD(void)
{
}

void HSUBPS(void)
{
}

void IDIV(void)
{
}

void IMUL(void)
{
}

void IN(void)
{
}

void INC(void)
{
}

void INCSSPD(void)
{
}

void INCSSPQ(void)
{
}

void INS(void)
{
}

void INSB(void)
{
}

void INSW(void)
{
}

void INSD(void)
{
}

void INSERTPS(void)
{
}

void INT_imm8(void)
{
}

void INTO(void)
{
}

void INT3(void)
{
}

void INT1(void)
{
}

void Jcc(void)
{
}

void JMP(void)
{
}

void LAHF(void)
{
}

void LAR(void)
{
}

void LDDQU(void)
{
}

void LDMXCSR(void)
{
}

void LDS(void)
{
}

void LES(void)
{
}

void LFS(void)
{
}

void LGS(void)
{
}

void LSS(void)
{
}

void LEA(void)
{
}

void LEAVE(void)
{
}

void LFENCE(void)
{
}

void LODS(void)
{
}

void LODSB(void)
{
}

void LODSW(void)
{
}

void LODSD(void)
{
}

void LODSQ(void)
{
}

void LOOP(void)
{
}

void LOOPcc(void)
{
}

void LSL(void)
{
}

void LZCNT(void)
{
}

void MASKMOVDQU(void)
{
}

void MASKMOVQ(void)
{
}

void MAXPD(void)
{
}

void MAXPS(void)
{
}

void MAXSD(void)
{
}

void MAXSS(void)
{
}

void MFENCE(void)
{
}

void MINPD(void)
{
}

void MINPS(void)
{
}

void MINSD(void)
{
}

void MINSS(void)
{
}

void MOV(void)
{
}

void MOVAPD(void)
{
}

void MOVAPS(void)
{
}

void MOVBE(void)
{
}

void MOVD(void)
{
}

void MOVQ(void)
{
}

void MOVDDUP(void)
{
}

void MOVDQA(void)
{
}

void MOVDQU(void)
{
}

void MOVDQ2Q(void)
{
}

void MOVHLPS(void)
{
}

void MOVHPD(void)
{
}

void MOVHPS(void)
{
}

void MOVLHPS(void)
{
}

void MOVLPD(void)
{
}

void MOVLPS(void)
{
}

void MOVMSKPD(void)
{
}

void MOVMSKPS(void)
{
}

void MOVNTDQA(void)
{
}

void MOVNTDQ(void)
{
}

void MOVNTI(void)
{
}

void MOVNTPD(void)
{
}

void MOVNTPS(void)
{
}

void MOVNTQ(void)
{
}

void MOVQ2(void)
{
}

void MOVQ2DQ(void)
{
}

void MOVS(void)
{
}

void MOVSB(void)
{
}

void MOVSW(void)
{
}

void MOVSD(void)
{
}

void MOVSQ(void)
{
}

void MOVSD2 (void)
{
}

void MOVSHDUP(void)
{
}

void MOVSLDUP(void)
{
}

void MOVSS(void)
{
}

void MOVSX(void)
{
}

void MOVSXD(void)
{
}

void MOVUPD(void)
{
}

void MOVUPS(void)
{
}

void MOVZX(void)
{
}

void MPSADBW(void)
{
}

void MUL(void)
{
}

void MULPD(void)
{
}

void MULPS(void)
{
}

void MULSD(void)
{
}

void MULSS(void)
{
}

void MULX(void)
{
}

void NEG(void)
{
}

void NOP(void)
{
}

void NOT(void)
{
}

void OR(void)
{
}

void ORPD(void)
{
}

void ORPS(void)
{
}

void OUT(void)
{
}

void OUTS(void)
{
}

void OUTSB(void)
{
}

void OUTSW(void)
{
}

void OUTSD(void)
{
}

void PABSB(void)
{
}

void PABSW(void)
{
}

void PABSD(void)
{
}

void PABSQ(void)
{
}

void PACKSSWB(void)
{
}

void PACKSSDW(void)
{
}

void PACKUSDW(void)
{
}

void PACKUSWB(void)
{
}

void PADDB(void)
{
}

void PADDW(void)
{
}

void PADDD(void)
{
}

void PADDQ(void)
{
}

void PADDSB(void)
{
}

void PADDSW(void)
{
}

void PADDUSB(void)
{
}

void PADDUSW(void)
{
}

void PALIGNR(void)
{
}

void PAND(void)
{
}

void PANDN(void)
{
}

void PAUSE(void)
{
}

void PAVGB(void)
{
}

void PAVGW(void)
{
}

void PBLENDVB(void)
{
}

void PBLENDW(void)
{
}

void PCLMULQDQ(void)
{
}

void PCMPEQB(void)
{
}

void PCMPEQW(void)
{
}

void PCMPEQD(void)
{
}

void PCMPEQQ(void)
{
}

void PCMPESTRI(void)
{
}

void PCMPESTRM(void)
{
}

void PCMPGTB(void)
{
}

void PCMPGTW(void)
{
}

void PCMPGTD(void)
{
}

void PCMPGTQ(void)
{
}

void PCMPISTRI(void)
{
}

void PCMPISTRM(void)
{
}

void PDEP(void)
{
}

void PEXT(void)
{
}

void PEXTRB(void)
{
}

void PEXTRD(void)
{
}

void PEXTRQ(void)
{
}

void PEXTRW(void)
{
}

void PHADDW(void)
{
}

void PHADDD(void)
{
}

void PHADDSW(void)
{
}

void PHMINPOSUW(void)
{
}

void PHSUBW(void)
{
}

void PHSUBD(void)
{
}

void PHSUBSW(void)
{
}

void PINSRB(void)
{
}

void PINSRD(void)
{
}

void PINSRQ(void)
{
}

void PINSRW(void)
{
}

void PMADDUBSW(void)
{
}

void PMADDWD(void)
{
}

void PMAXSB(void)
{
}

void PMAXSW(void)
{
}

void PMAXSD(void)
{
}

void PMAXUB(void)
{
}

void PMAXUW(void)
{
}

void PMAXUD(void)
{
}

void PMINSB(void)
{
}

void PMINSW(void)
{
}

void PMINSD(void)
{
}

void PMINUB(void)
{
}

void PMINUW(void)
{
}

void PMINUD(void)
{
}

void PMOVMSKB(void)
{
}

void PMOVSX(void)
{
}

void PMOVZX(void)
{
}

void PMULDQ(void)
{
}

void PMULHRSW(void)
{
}

void PMULHUW(void)
{
}

void PMULHW(void)
{
}

void PMULLD(void)
{
}

void PMULLW(void)
{
}

void PMULUDQ(void)
{
}

void POP(void)
{
}

void POPA(void)
{
}

void POPAD(void)
{
}

void POPCNT(void)
{
}

void POPF(void)
{
}

void POPFD(void)
{
}

void POPFQ(void)
{
}

void POR(void)
{
}

void PREFETCHT0(void)
{
}

void PREFETCHT1(void)
{
}

void PREFETCHT2(void)
{
}

void PREFETCHNTA(void)
{
}

void PSADBW(void)
{
}

void PSHUFB(void)
{
}

void PSHUFD(void)
{
}

void PSHUFHW(void)
{
}

void PSHUFLW(void)
{
}

void PSHUFW(void)
{
}

void PSIGNB(void)
{
}

void PSIGNW(void)
{
}

void PSIGND(void)
{
}

void PSLLDQ(void)
{
}

void PSLLW(void)
{
}

void PSLLD(void)
{
}

void PSLLQ(void)
{
}

void PSRAW(void)
{
}

void PSRAD(void)
{
}

void PSRLDQ(void)
{
}

void PSRLW(void)
{
}

void PSRLD(void)
{
}

void PSRLQ(void)
{
}

void PSUBB(void)
{
}

void PSUBW(void)
{
}

void PSUBD(void)
{
}

void PSUBQ(void)
{
}

void PSUBSB(void)
{
}

void PSUBSW(void)
{
}

void PSUBUSB(void)
{
}

void PSUBUSW(void)
{
}

void PTEST(void)
{
}

void PUNPCKHBW(void)
{
}

void PUNPCKHWD(void)
{
}

void PUNPCKHDQ(void)
{
}

void PUNPCKHQDQ(void)
{
}

void PUNPCKLBW(void)
{
}

void PUNPCKLWD(void)
{
}

void PUNPCKLDQ(void)
{
}

void PUNPCKLQDQ(void)
{
}

void PUSH(void)
{
}

void PUSHA(void)
{
}

void PUSHAD(void)
{
}

void PUSHF(void)
{
}

void PUSHFD(void)
{
}

void PUSHFQ(void)
{
}

void PXOR(void)
{
}

void RCPPS(void)
{
}

void RCPSS(void)
{
}

void RDRAND(void)
{
}

void RDSEED(void)
{
}

void RDSSPD(void)
{
}

void RDSSPQ(void)
{
}

void RDTSC(void)
{
}

void RDTSCP(void)
{
}

void RET(void)
{
}

void RORX(void)
{
}

void ROUNDPD(void)
{
}

void ROUNDPS(void)
{
}

void ROUNDSD(void)
{
}

void ROUNDSS(void)
{
}

void RSM(void)
{
}

void RSQRTPS(void)
{
}

void RSQRTSS(void)
{
}

void RSTORSSP(void)
{
}

void SAHF(void)
{
}

void SAL(void)
{
}

void SAR(void)
{
}

void SHL(void)
{
}

void SHR(void)
{
}

void SARX(void)
{
}

void SHLX(void)
{
}

void SHRX(void)
{
}

void SAVEPREVSSP(void)
{
}

void SBB(void)
{
}

void SCAS(void)
{
}

void SCASB(void)
{
}

void SCASW(void)
{
}

void SCASD(void)
{
}

void SETcc(void)
{
}

void SFENCE(void)
{
}

void SHLD(void)
{
}

void SHRD(void)
{
}

void SHUFPD(void)
{
}

void SHUFPS(void)
{
}

void SLDT(void)
{
}

void SQRTPD(void)
{
}

void SQRTPS(void)
{
}

void SQRTSD(void)
{
}

void SQRTSS(void)
{
}

void STAC(void)
{
}

void STC(void)
{
}

void STD(void)
{
}

void STI(void)
{
}

void STMXCSR(void)
{
}

void STOS(void)
{
}

void STOSB(void)
{
}

void STOSW(void)
{
}

void STOSD(void)
{
}

void STR(void)
{
}

void SUB(void)
{
}

void SUBPD(void)
{
}

void SUBPS(void)
{
}

void SUBSD(void)
{
}

void SUBSS(void)
{
}

void SYSENTER(void)
{
}

void TEST(void)
{
}

void TZCNT(void)
{
}

void UCOMISD(void)
{
}

void UCOMISS(void)
{
}

void UD0(void)
{
}

void UD1(void)
{
}

void UD2(void)
{
}

void UNPCKHPD(void)
{
}

void UNPCKHPS(void)
{
}

void UNPCKLPD(void)
{
}

void UNPCKLPS(void)
{
}

void VERR(void)
{
}

void VERW(void)
{
}

void WAIT(void)
{
}

void FWAIT(void)
{
}

void XADD(void)
{
}

void XCHG(void)
{
}

void XGETBV(void)
{
}

void XLAT(void)
{
}

void XLATB(void)
{
}

void XOR(void)
{
}

void XORPD(void)
{
}

void XORPS(void)
{
}
