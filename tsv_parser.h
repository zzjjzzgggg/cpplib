#ifndef IOUTILS_H
#define IOUTILS_H

class TSVParser {
private:
  TSsFmt SsFmt;
  bool SkipLeadBlanks, SkipCmt, SkipEmptyFld;
  uint64 LineCnt;
  char SplitCh;
  TChA LineStr;
  TVec<char*> FldV;
  PSIn FInPt;
  bool Silent;

public:
  TSVParser(const TStr& FNm, const bool& Silent = true,
            const TSsFmt _SsFmt = ssfTabSep,
            const bool& _SkipLeadBlanks = true,
            const bool& _SkipCmt = true,
            const bool& _SkipEmptyFld = true);
  TSVParser(const TStr& FNm, const TSsFmt _SsFmt)
      : TSVParser(FNm, true, _SsFmt) {}
  TSVParser(const TStr& FNm, const char& Separator,
            const bool& Silent = true,
            const bool& _SkipLeadBlanks = false,
            const bool& _SkipCmt = true,
            const bool& _SkipEmptyFld = false);
  ~TSVParser();
  bool Next();
  int Len() const { return FldV.Len(); }
  int GetFlds() const { return Len(); }
  uint64 GetLineNo() const { return LineCnt; }
  bool IsCmt() const {
    return Len() > 0 && GetFld(0)[0] == '#';
  }
  bool Eof() const { return FInPt->Eof(); }
  const TChA& GetLnStr() const { return LineStr; }
  void ToLc();
  const char* GetFld(const int& FldN) const {
    return FldV[FldN];
  }
  char* GetFld(const int& FldN) { return FldV[FldN]; }
  const char* operator[](const int& FldN) const {
    return FldV[FldN];
  }
  char* operator[](const int& FldN) { return FldV[FldN]; }
  bool GetInt(const int& FldN, int& Val) const;
  int GetInt(const int& FldN) const {
    int Val = 0;
    IAssertR(
        GetInt(FldN, Val),
        TStr::Fmt("Field %d not INT.\n%s", FldN, DumpStr())
            .CStr());
    return Val;
  }
  bool IsInt(const int& FldN) const {
    int v;
    return GetInt(FldN, v);
  }
  bool GetFlt(const int& FldN, double& Val) const;
  bool IsFlt(const int& FldN) const {
    double v;
    return GetFlt(FldN, v);
  }
  double GetFlt(const int& FldN) const {
    double Val = 0.0;
    IAssert(GetFlt(FldN, Val));
    return Val;
  }
  const char* DumpStr() const;
};

#endif
