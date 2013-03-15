#do the WZ auxiliar rootfile
root.exe -b -l -q doWZ.C+\(8\,0\)

#and the WJets
root.exe -b -l -q doFakes.C+\(8\,0\)

root.exe -b -l -q chain.C+\(8\,0\,125\,0\)
root.exe -b -l -q chain.C+\(8\,1\,125\,0\)
root.exe -b -l -q chain.C+\(8\,2\,125\,0\)
root.exe -b -l -q chain.C+\(8\,3\,125\,0\)
root.exe -b -l -q chain.C+\(8\,4\,125\,0\)
root.exe -b -l -q chain.C+\(8\,5\,125\,0\)
#systematics, Stat Down
root.exe -b -l -q syst.C+\(8\,1\,125\,1\,0\,0\)
root.exe -b -l -q syst.C+\(8\,2\,125\,1\,0\,0\)
root.exe -b -l -q syst.C+\(8\,3\,125\,1\,0\,0\)
root.exe -b -l -q syst.C+\(8\,4\,125\,1\,0\,0\)
root.exe -b -l -q syst.C+\(8\,5\,125\,1\,0\,0\)
#Stat Up
root.exe -b -l -q syst.C+\(8\,1\,125\,1\,1\,0\)
root.exe -b -l -q syst.C+\(8\,2\,125\,1\,1\,0\)
root.exe -b -l -q syst.C+\(8\,3\,125\,1\,1\,0\)
root.exe -b -l -q syst.C+\(8\,4\,125\,1\,1\,0\)
root.exe -b -l -q syst.C+\(8\,5\,125\,1\,1\,0\)
#systematics, JES Down
root.exe -b -l -q syst.C+\(8\,1\,125\,2\,0\,0\)
root.exe -b -l -q syst.C+\(8\,2\,125\,2\,0\,0\)
root.exe -b -l -q syst.C+\(8\,3\,125\,2\,0\,0\)
root.exe -b -l -q syst.C+\(8\,4\,125\,2\,0\,0\)
#JES Up
root.exe -b -l -q syst.C+\(8\,1\,125\,2\,1\,0\)
root.exe -b -l -q syst.C+\(8\,2\,125\,2\,1\,0\)
root.exe -b -l -q syst.C+\(8\,3\,125\,2\,1\,0\)
root.exe -b -l -q syst.C+\(8\,4\,125\,2\,1\,0\)
#systematics, PU Down
root.exe -b -l -q syst.C+\(8\,1\,125\,3\,0\,0\)
root.exe -b -l -q syst.C+\(8\,2\,125\,3\,0\,0\)
root.exe -b -l -q syst.C+\(8\,3\,125\,3\,0\,0\)
root.exe -b -l -q syst.C+\(8\,4\,125\,3\,0\,0\)
#PU Up
root.exe -b -l -q syst.C+\(8\,1\,125\,3\,1\,0\)
root.exe -b -l -q syst.C+\(8\,2\,125\,3\,1\,0\)
root.exe -b -l -q syst.C+\(8\,3\,125\,3\,1\,0\)
root.exe -b -l -q syst.C+\(8\,4\,125\,3\,1\,0\)
#systematics, MET Down
root.exe -b -l -q syst.C+\(8\,1\,125\,4\,0\,0\)
root.exe -b -l -q syst.C+\(8\,2\,125\,4\,0\,0\)
root.exe -b -l -q syst.C+\(8\,3\,125\,4\,0\,0\)
root.exe -b -l -q syst.C+\(8\,4\,125\,4\,0\,0\)
#MET Up
root.exe -b -l -q syst.C+\(8\,1\,125\,4\,1\,0\)
root.exe -b -l -q syst.C+\(8\,2\,125\,4\,1\,0\)
root.exe -b -l -q syst.C+\(8\,3\,125\,4\,1\,0\)
root.exe -b -l -q syst.C+\(8\,4\,125\,4\,1\,0\)
#systematics, Lepton Down
root.exe -b -l -q syst.C+\(8\,1\,125\,5\,0\,0\)
root.exe -b -l -q syst.C+\(8\,2\,125\,5\,0\,0\)
root.exe -b -l -q syst.C+\(8\,3\,125\,5\,0\,0\)
root.exe -b -l -q syst.C+\(8\,4\,125\,5\,0\,0\)
#Lepton Up
root.exe -b -l -q syst.C+\(8\,1\,125\,5\,1\,0\)
root.exe -b -l -q syst.C+\(8\,2\,125\,5\,1\,0\)
root.exe -b -l -q syst.C+\(8\,3\,125\,5\,1\,0\)
root.exe -b -l -q syst.C+\(8\,4\,125\,5\,1\,0\)
#systematics, Lepton Eff Down
root.exe -b -l -q syst.C+\(8\,1\,125\,6\,0\,0\)
root.exe -b -l -q syst.C+\(8\,2\,125\,6\,0\,0\)
root.exe -b -l -q syst.C+\(8\,3\,125\,6\,0\,0\)
root.exe -b -l -q syst.C+\(8\,4\,125\,6\,0\,0\)
#Lepton Eff Up
root.exe -b -l -q syst.C+\(8\,1\,125\,6\,1\,0\)
root.exe -b -l -q syst.C+\(8\,2\,125\,6\,1\,0\)
root.exe -b -l -q syst.C+\(8\,3\,125\,6\,1\,0\)
root.exe -b -l -q syst.C+\(8\,4\,125\,6\,1\,0\)
#WZ Down and Up
root.exe -b -l -q syst.C+\(8\,2\,125\,7\,0\,0\)
root.exe -b -l -q syst.C+\(8\,2\,125\,7\,1\,0\)
#Fakes! Down and Up
root.exe -b -l -q syst.C+\(8\,5\,125\,8\,0\,0\)
root.exe -b -l -q syst.C+\(8\,5\,125\,8\,1\,0\)


root.exe -b -l -q datacardmaker8.C+\(125\,0\)

#do the WZ auxiliar rootfile
root.exe -b -l -q doWZ.C+\(7\,0\)

#and the WJets
root.exe -b -l -q doFakes.C+\(7\,0\)

root.exe -b -l -q chain.C+\(7\,0\,125\,0\)
root.exe -b -l -q chain.C+\(7\,1\,125\,0\)
root.exe -b -l -q chain.C+\(7\,2\,125\,0\)
root.exe -b -l -q chain.C+\(7\,3\,125\,0\)
root.exe -b -l -q chain.C+\(7\,5\,125\,0\)
#systematics, Stat Down
root.exe -b -l -q syst.C+\(7\,1\,125\,1\,0\,0\)
root.exe -b -l -q syst.C+\(7\,2\,125\,1\,0\,0\)
root.exe -b -l -q syst.C+\(7\,3\,125\,1\,0\,0\)
root.exe -b -l -q syst.C+\(7\,5\,125\,1\,0\,0\)
#Stat Up
root.exe -b -l -q syst.C+\(7\,1\,125\,1\,1\,0\)
root.exe -b -l -q syst.C+\(7\,2\,125\,1\,1\,0\)
root.exe -b -l -q syst.C+\(7\,3\,125\,1\,1\,0\)
root.exe -b -l -q syst.C+\(7\,5\,125\,1\,1\,0\)
#systematics, JES Down
root.exe -b -l -q syst.C+\(7\,1\,125\,2\,0\,0\)
root.exe -b -l -q syst.C+\(7\,2\,125\,2\,0\,0\)
root.exe -b -l -q syst.C+\(7\,3\,125\,2\,0\,0\)
#JES Up
root.exe -b -l -q syst.C+\(7\,1\,125\,2\,1\,0\)
root.exe -b -l -q syst.C+\(7\,2\,125\,2\,1\,0\)
root.exe -b -l -q syst.C+\(7\,3\,125\,2\,1\,0\)
#systematics, PU Down
root.exe -b -l -q syst.C+\(7\,1\,125\,3\,0\,0\)
root.exe -b -l -q syst.C+\(7\,2\,125\,3\,0\,0\)
root.exe -b -l -q syst.C+\(7\,3\,125\,3\,0\,0\)
#PU Up
root.exe -b -l -q syst.C+\(7\,1\,125\,3\,1\,0\)
root.exe -b -l -q syst.C+\(7\,2\,125\,3\,1\,0\)
root.exe -b -l -q syst.C+\(7\,3\,125\,3\,1\,0\)
#systematics, MET Down
root.exe -b -l -q syst.C+\(7\,1\,125\,4\,0\,0\)
root.exe -b -l -q syst.C+\(7\,2\,125\,4\,0\,0\)
root.exe -b -l -q syst.C+\(7\,3\,125\,4\,0\,0\)
#MET Up
root.exe -b -l -q syst.C+\(7\,1\,125\,4\,1\,0\)
root.exe -b -l -q syst.C+\(7\,2\,125\,4\,1\,0\)
root.exe -b -l -q syst.C+\(7\,3\,125\,4\,1\,0\)
#systematics, Lepton Down
root.exe -b -l -q syst.C+\(7\,1\,125\,5\,0\,0\)
root.exe -b -l -q syst.C+\(7\,2\,125\,5\,0\,0\)
root.exe -b -l -q syst.C+\(7\,3\,125\,5\,0\,0\)
#Lepton Up
root.exe -b -l -q syst.C+\(7\,1\,125\,5\,1\,0\)
root.exe -b -l -q syst.C+\(7\,2\,125\,5\,1\,0\)
root.exe -b -l -q syst.C+\(7\,3\,125\,5\,1\,0\)
#systematics, Lepton Eff Down
root.exe -b -l -q syst.C+\(7\,1\,125\,6\,0\,0\)
root.exe -b -l -q syst.C+\(7\,2\,125\,6\,0\,0\)
root.exe -b -l -q syst.C+\(7\,3\,125\,6\,0\,0\)
#Lepton Eff Up
root.exe -b -l -q syst.C+\(7\,1\,125\,6\,1\,0\)
root.exe -b -l -q syst.C+\(7\,2\,125\,6\,1\,0\)
root.exe -b -l -q syst.C+\(7\,3\,125\,6\,1\,0\)
#WZ Down and Up
root.exe -b -l -q syst.C+\(7\,2\,125\,7\,0\,0\)
root.exe -b -l -q syst.C+\(7\,2\,125\,7\,1\,0\)
#Fakes! Down and Up
root.exe -b -l -q syst.C+\(7\,5\,125\,8\,0\,0\)
root.exe -b -l -q syst.C+\(7\,5\,125\,8\,1\,0\)


root.exe -b -l -q datacardmaker7.C+\(125\,0\)


