/////////////////////////////////////////////////
// Fast-Spread-Sheet-Parser
TSVParser::TSVParser(const TStr& FNm, const bool& Silent,
                     const TSsFmt _SsFmt,
                     const bool& _SkipLeadBlanks,
                     const bool& _SkipCmt,
                     const bool& _SkipEmptyFld)
    : SsFmt(_SsFmt), SkipLeadBlanks(_SkipLeadBlanks),
      SkipCmt(_SkipCmt), SkipEmptyFld(_SkipEmptyFld),
      LineCnt(0), SplitCh('\t'), FldV(), FInPt(NULL),
      Silent(Silent) {
  if (TZipIn::IsZipExt(FNm.GetFExt()))
    FInPt = TZipIn::New(FNm, Silent);
  else
    FInPt = TFIn::New(FNm);

  // Bf = new char [BfLen];
  switch (SsFmt) {
    case ssfTabSep:
      SplitCh = '\t';
      break;
    case ssfCommaSep:
      SplitCh = ',';
      break;
    case ssfSemicolonSep:
      SplitCh = ';';
      break;
    case ssfSpaceSep:
      SplitCh = ' ';
      break;
    case ssfWhiteSep:
      SplitCh = ' ';
      break;
    default:
      FailR("Unknown separator character.");
      break;
  }
}

TSVParser::TSVParser(const TStr& FNm, const char& Separator,
                     const bool& Silent,
                     const bool& _SkipLeadBlanks,
                     const bool& _SkipCmt,
                     const bool& _SkipEmptyFld)
    : SsFmt(ssfSpaceSep), SkipLeadBlanks(_SkipLeadBlanks),
      SkipCmt(_SkipCmt), SkipEmptyFld(_SkipEmptyFld),
      LineCnt(0), /*Bf(NULL),*/ SplitCh('\t'), FldV(),
      FInPt(NULL), Silent(Silent) {
  if (TZipIn::IsZipExt(FNm.GetFExt()))
    FInPt = TZipIn::New(FNm, Silent);
  else
    FInPt = TFIn::New(FNm);
  SplitCh = Separator;
}

TSVParser::~TSVParser() {
  // if (Bf != NULL) { delete [] Bf; }
}

/*bool TSVParser::Next() { // split on SplitCh
  const char* EndBf = Bf+BfLen-1;
  memset(Bf, 0, BfLen);
  char *cur = Bf, *last = Bf;
  FldV.Clr(false);
  TSIn& FIn = *FInPt;
  if (SkipLeadBlanks) { // skip leadning blanks
  while (! FIn.Eof() && cur < EndBf && (FIn.PeekCh()=='\t' ||
  FIn.PeekCh()==' ')) { FIn.GetCh(); }
  }
  while (! FIn.Eof() && cur < EndBf) {
  if (SsFmt == ssfWhiteSep) {
  while (! FIn.Eof() && cur < EndBf && !
  TCh::IsWs(*cur=FIn.GetCh())) { cur++; }
  } else {
  while (! FIn.Eof() && cur < EndBf &&
  (*cur=FIn.GetCh())!=SplitCh && *cur!='\r' && *cur!='\n') {
  cur++; }
  }
  if (*cur=='\r' || *cur=='\n') {
  *cur = 0; cur++;
  if (*last) { FldV.Add(last); }
  last = cur;
  break;
  }
  *cur = 0;  cur++;
  FldV.Add(last);  last = cur;
  if (SkipEmpty && strlen(FldV.Last())==0) { FldV.DelLast(); }
  }
  if (SkipEmpty && FldV.Len()>0 && strlen(FldV.Last())==0) {
  FldV.DelLast();
  }
  LineCnt++;
  if (! FldV.Empty() && cur < EndBf) {
  if (SkipCmt && IsCmt()) { return Next(); }
  else { return true; } }
  else if (! FIn.Eof() && ! SkipEmpty) { return true; }
  else { return false; }
  }*/

bool TSVParser::Next() {  // split on SplitCh
  FldV.Clr(false);
  LineStr.Clr();
  FldV.Clr();
  LineCnt++;
  if (!FInPt->GetNextLn(LineStr)) return false;
  if (SkipCmt && LineStr.Len() > 0 && LineStr[0] == '#')
    return Next();

  char* cur = LineStr.CStr();
  if (SkipLeadBlanks) {  // skip leadning blanks
    while (*cur && TCh::IsWs(*cur)) cur++;
  }
  char* last = cur;
  while (*cur) {
    if (SsFmt == ssfWhiteSep) {
      while (*cur && !TCh::IsWs(*cur)) cur++;
    } else {
      while (*cur && *cur != SplitCh) cur++;
    }
    if (*cur == 0) break;

    *cur = 0;
    cur++;
    FldV.Add(last);
    last = cur;
    if (SkipEmptyFld && strlen(FldV.Last()) == 0)
      FldV.DelLast();  // skip empty fields
  }
  FldV.Add(last);  // add last field
  if (SkipEmptyFld && FldV.Empty())
    return Next();  // skip empty lines
  return true;

  // const char* EndBf = Bf+BfLen-1;
  // memset(Bf, 0, BfLen);
  // char *cur = Bf, *last = Bf;
  /*if (SkipLeadBlanks) { // skip leadning blanks
    while (! FIn.Eof() && cur < EndBf && (FIn.PeekCh()=='\t'
    || FIn.PeekCh()==' ')) { FIn.GetCh(); }
    }
    while (! FIn.Eof() && cur < EndBf) {
    if (SsFmt == ssfWhiteSep) {
    while (! FIn.Eof() && cur < EndBf && !
    TCh::IsWs(*cur=FIn.GetCh())) { cur++; }
    } else {
    while (! FIn.Eof() && cur < EndBf &&
    (*cur=FIn.GetCh())!=SplitCh && *cur!='\r' && *cur!='\n') {
    cur++; }
    }
    if (*cur=='\r' || *cur=='\n') {
    if (! FIn.Eof()) { // read the remaining of the line
    if (*cur == '\r' && FIn.PeekCh()=='\n') { FIn.GetCh(); }
    else if (*cur == '\n' && FIn.PeekCh()=='\r') {
    FIn.GetCh(); }
    }
    *cur = 0; cur++;
    FldV.Add(last);
    last = cur;
    break;
    }
    *cur = 0;  cur++;
    FldV.Add(last);  last = cur;
    if (SkipEmptyFld && strlen(FldV.Last())==0) {
    FldV.DelLast(); } // skip empty fields
    }
    LineCnt++;
    if (SkipCmt && IsCmt() && ! FIn.Eof()) { return Next(); }
    if (FldV.Len() == 1 && strlen(FldV[0])==0) { FldV.Clr();
    return true; }
    if (SkipEmptyFld && FldV.Len()>0 &&
    strlen(FldV.Last())==0) { FldV.DelLast(); }
    return ! FIn.Eof() || ! FldV.Empty();
    //if (SkipEmptyFld && FldV.Empty() && ! FIn.Eof()) {
    return Next(); } // skip empty line
    */
}

void TSVParser::ToLc() {
  for (int f = 0; f < FldV.Len(); f++) {
    for (char* c = FldV[f]; *c; c++) *c = tolower(*c);
  }
}

bool TSVParser::GetInt(const int& FldN, int& Val) const {
  // parsing format {ws} [+/-] +{ddd}
  int _Val = -1;
  bool Minus = false;
  const char* c = GetFld(FldN);
  while (TCh::IsWs(*c)) c++;
  if (*c == '-') {
    Minus = true;
    c++;
  }
  if (!TCh::IsNum(*c)) return false;
  _Val = TCh::GetNum(*c);
  c++;
  while (TCh::IsNum(*c)) {
    _Val = 10 * _Val + TCh::GetNum(*c);
    c++;
  }
  if (Minus) _Val = -_Val;
  if (*c != 0) return false;
  Val = _Val;
  return true;
}

bool TSVParser::GetFlt(const int& FldN, double& Val) const {
  // parsing format {ws} [+/-] +{d} ([.]{d}) ([E|e] [+/-]
  // +{d})
  const char* c = GetFld(FldN);
  while (TCh::IsWs(*c)) c++;
  if (*c == '+' || *c == '-') c++;
  if (!TCh::IsNum(*c) && *c != '.') return false;
  while (TCh::IsNum(*c)) c++;
  if (*c == '.') {
    c++;
    while (TCh::IsNum(*c)) c++;
  }
  if (*c == 'e' || *c == 'E') {
    c++;
    if (*c == '+' || *c == '-') c++;
    if (!TCh::IsNum(*c)) return false;
    while (TCh::IsNum(*c)) c++;
  }
  if (*c != 0) return false;
  Val = atof(GetFld(FldN));
  return true;
}

const char* TSVParser::DumpStr() const {
  static TChA ChA(10 * 1024);
  ChA.Clr();
  for (int i = 0; i < FldV.Len(); i++)
    ChA += TStr::Fmt("  %d: '%s'\n", i, FldV[i]);
  return ChA.CStr();
}
