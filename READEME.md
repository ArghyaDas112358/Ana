# Plan for the code base 
built along-side ChatGPT-5.2 Extended Thinking
```bash
src/Ana/
  interface/          # headers (CMSSW convention; "include/" also ok)
    Ana/
      Types.h         # enums/structs (GenDecay, GenMatchingResult)
      PDG.h           # PDG maps + helpers
      StatusFlags.h   # bit helpers (isLastCopy, fromHardProcess, ...)
      Kinematics.h    # deltaR/deltaPhi, overflowProtected, getP4
      Collections.h   # computeP4Vec, DefineP4, GetGenParticles, ...
      GenMatching.h   # findMothers/findDescendants/DecayGenMatching/MatchVBFJets decls
      RecoMatching.h
  src/
    GenMatching.cc    # definitions (non-template, non-inline)
    Collections.cc
    RecoMatching.cc
  macros/
    LoadAnaRDF.C      # a small ROOT macro that includes headers / loads lib
```