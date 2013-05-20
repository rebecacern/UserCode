#do the WZ auxiliar rootfile
root.exe -b -l -q doWZ.C+\(8\,4\)

#and the WJets
root.exe -b -l -q doFakes.C+\(8\,4\)

root.exe -b -l -q chain.C+\(8\,0\,200\,4\)
root.exe -b -l -q chain.C+\(8\,1\,200\,4\)
root.exe -b -l -q chain.C+\(8\,2\,200\,4\)
root.exe -b -l -q chain.C+\(8\,3\,200\,4\)
root.exe -b -l -q chain.C+\(8\,4\,200\,4\)
root.exe -b -l -q chain.C+\(8\,5\,200\,4\)
#systematics, Stat Down
root.exe -b -l -q syst.C+\(8\,1\,200\,1\,0\,4\)
root.exe -b -l -q syst.C+\(8\,2\,200\,1\,0\,4\)
root.exe -b -l -q syst.C+\(8\,3\,200\,1\,0\,4\)
root.exe -b -l -q syst.C+\(8\,4\,200\,1\,0\,4\)
root.exe -b -l -q syst.C+\(8\,5\,200\,1\,0\,4\)
#Stat Up
root.exe -b -l -q syst.C+\(8\,1\,200\,1\,1\,4\)
root.exe -b -l -q syst.C+\(8\,2\,200\,1\,1\,4\)
root.exe -b -l -q syst.C+\(8\,3\,200\,1\,1\,4\)
root.exe -b -l -q syst.C+\(8\,4\,200\,1\,1\,4\)
root.exe -b -l -q syst.C+\(8\,5\,200\,1\,1\,4\)
#systematics, JES Down
root.exe -b -l -q syst.C+\(8\,1\,200\,2\,0\,4\)
root.exe -b -l -q syst.C+\(8\,2\,200\,2\,0\,4\)
root.exe -b -l -q syst.C+\(8\,3\,200\,2\,0\,4\)
root.exe -b -l -q syst.C+\(8\,4\,200\,2\,0\,4\)
#JES Up
root.exe -b -l -q syst.C+\(8\,1\,200\,2\,1\,4\)
root.exe -b -l -q syst.C+\(8\,2\,200\,2\,1\,4\)
root.exe -b -l -q syst.C+\(8\,3\,200\,2\,1\,4\)
root.exe -b -l -q syst.C+\(8\,4\,200\,2\,1\,4\)
#systematics, PU Down
#root.exe -b -l -q syst.C+\(8\,1\,200\,3\,0\,4\)
#root.exe -b -l -q syst.C+\(8\,2\,200\,3\,0\,4\)
#root.exe -b -l -q syst.C+\(8\,3\,200\,3\,0\,4\)
#root.exe -b -l -q syst.C+\(8\,4\,200\,3\,0\,4\)
#PU Up
#root.exe -b -l -q syst.C+\(8\,1\,200\,3\,1\,4\)
#root.exe -b -l -q syst.C+\(8\,2\,200\,3\,1\,4\)
#root.exe -b -l -q syst.C+\(8\,3\,200\,3\,1\,4\)
#root.exe -b -l -q syst.C+\(8\,4\,200\,3\,1\,4\)
#systematics, MET Down
#root.exe -b -l -q syst.C+\(8\,1\,200\,4\,0\,4\)
#root.exe -b -l -q syst.C+\(8\,2\,200\,4\,0\,4\)
#root.exe -b -l -q syst.C+\(8\,3\,200\,4\,0\,4\)
#root.exe -b -l -q syst.C+\(8\,4\,200\,4\,0\,4\)
#MET Up
#root.exe -b -l -q syst.C+\(8\,1\,200\,4\,1\,4\)
#root.exe -b -l -q syst.C+\(8\,2\,200\,4\,1\,4\)
#root.exe -b -l -q syst.C+\(8\,3\,200\,4\,1\,4\)
#root.exe -b -l -q syst.C+\(8\,4\,200\,4\,1\,4\)
#systematics, Lepton Down
#root.exe -b -l -q syst.C+\(8\,1\,200\,5\,0\,4\)
#root.exe -b -l -q syst.C+\(8\,2\,200\,5\,0\,4\)
#root.exe -b -l -q syst.C+\(8\,3\,200\,5\,0\,4\)
#root.exe -b -l -q syst.C+\(8\,4\,200\,5\,0\,4\)
#Lepton Up
#root.exe -b -l -q syst.C+\(8\,1\,200\,5\,1\,4\)
#root.exe -b -l -q syst.C+\(8\,2\,200\,5\,1\,4\)
#root.exe -b -l -q syst.C+\(8\,3\,200\,5\,1\,4\)
#root.exe -b -l -q syst.C+\(8\,4\,200\,5\,1\,4\)
#systematics, Lepton Eff Down
root.exe -b -l -q syst.C+\(8\,1\,200\,6\,0\,4\)
root.exe -b -l -q syst.C+\(8\,2\,200\,6\,0\,4\)
root.exe -b -l -q syst.C+\(8\,3\,200\,6\,0\,4\)
root.exe -b -l -q syst.C+\(8\,4\,200\,6\,0\,4\)
#Lepton Eff Up
root.exe -b -l -q syst.C+\(8\,1\,200\,6\,1\,4\)
root.exe -b -l -q syst.C+\(8\,2\,200\,6\,1\,4\)
root.exe -b -l -q syst.C+\(8\,3\,200\,6\,1\,4\)
root.exe -b -l -q syst.C+\(8\,4\,200\,6\,1\,4\)
#WZ Down and Up
root.exe -b -l -q syst.C+\(8\,2\,200\,7\,0\,4\)
root.exe -b -l -q syst.C+\(8\,2\,200\,7\,1\,4\)
#Fakes! Down and Up
root.exe -b -l -q syst.C+\(8\,5\,200\,8\,0\,4\)
root.exe -b -l -q syst.C+\(8\,5\,200\,8\,1\,4\)


root.exe -b -l -q datacardmaker8.C+\(200\,4\)

#do the WZ auxiliar rootfile
root.exe -b -l -q doWZ.C+\(7\,4\)

#and the WJets
root.exe -b -l -q doFakes.C+\(7\,4\)

root.exe -b -l -q chain.C+\(7\,0\,200\,4\)
root.exe -b -l -q chain.C+\(7\,1\,200\,4\)
root.exe -b -l -q chain.C+\(7\,2\,200\,4\)
root.exe -b -l -q chain.C+\(7\,3\,200\,4\)
root.exe -b -l -q chain.C+\(7\,5\,200\,4\)
#systematics, Stat Down
root.exe -b -l -q syst.C+\(7\,1\,200\,1\,0\,4\)
root.exe -b -l -q syst.C+\(7\,2\,200\,1\,0\,4\)
root.exe -b -l -q syst.C+\(7\,3\,200\,1\,0\,4\)
root.exe -b -l -q syst.C+\(7\,5\,200\,1\,0\,4\)
#Stat Up
root.exe -b -l -q syst.C+\(7\,1\,200\,1\,1\,4\)
root.exe -b -l -q syst.C+\(7\,2\,200\,1\,1\,4\)
root.exe -b -l -q syst.C+\(7\,3\,200\,1\,1\,4\)
root.exe -b -l -q syst.C+\(7\,5\,200\,1\,1\,4\)
#systematics, JES Down
root.exe -b -l -q syst.C+\(7\,1\,200\,2\,0\,4\)
root.exe -b -l -q syst.C+\(7\,2\,200\,2\,0\,4\)
root.exe -b -l -q syst.C+\(7\,3\,200\,2\,0\,4\)
#JES Up
root.exe -b -l -q syst.C+\(7\,1\,200\,2\,1\,4\)
root.exe -b -l -q syst.C+\(7\,2\,200\,2\,1\,4\)
root.exe -b -l -q syst.C+\(7\,3\,200\,2\,1\,4\)
#systematics, PU Down
#root.exe -b -l -q syst.C+\(7\,1\,200\,3\,0\,4\)
#root.exe -b -l -q syst.C+\(7\,2\,200\,3\,0\,4\)
#root.exe -b -l -q syst.C+\(7\,3\,200\,3\,0\,4\)
#PU Up
root.exe -b -l -q syst.C+\(7\,1\,200\,3\,1\,4\)
root.exe -b -l -q syst.C+\(7\,2\,200\,3\,1\,4\)
root.exe -b -l -q syst.C+\(7\,3\,200\,3\,1\,4\)
#systematics, MET Down
#root.exe -b -l -q syst.C+\(7\,1\,200\,4\,0\,4\)
#root.exe -b -l -q syst.C+\(7\,2\,200\,4\,0\,4\)
#root.exe -b -l -q syst.C+\(7\,3\,200\,4\,0\,4\)
#MET Up
#root.exe -b -l -q syst.C+\(7\,1\,200\,4\,1\,4\)
#root.exe -b -l -q syst.C+\(7\,2\,200\,4\,1\,4\)
#root.exe -b -l -q syst.C+\(7\,3\,200\,4\,1\,4\)
#systematics, Lepton Down
#root.exe -b -l -q syst.C+\(7\,1\,200\,5\,0\,4\)
#root.exe -b -l -q syst.C+\(7\,2\,200\,5\,0\,4\)
#root.exe -b -l -q syst.C+\(7\,3\,200\,5\,0\,4\)
#Lepton Up
#root.exe -b -l -q syst.C+\(7\,1\,200\,5\,1\,4\)
#root.exe -b -l -q syst.C+\(7\,2\,200\,5\,1\,4\)
#root.exe -b -l -q syst.C+\(7\,3\,200\,5\,1\,4\)
#systematics, Lepton Eff Down
root.exe -b -l -q syst.C+\(7\,1\,200\,6\,0\,4\)
root.exe -b -l -q syst.C+\(7\,2\,200\,6\,0\,4\)
root.exe -b -l -q syst.C+\(7\,3\,200\,6\,0\,4\)
#Lepton Eff Up
root.exe -b -l -q syst.C+\(7\,1\,200\,6\,1\,4\)
root.exe -b -l -q syst.C+\(7\,2\,200\,6\,1\,4\)
root.exe -b -l -q syst.C+\(7\,3\,200\,6\,1\,4\)
#WZ Down and Up
root.exe -b -l -q syst.C+\(7\,2\,200\,7\,0\,4\)
root.exe -b -l -q syst.C+\(7\,2\,200\,7\,1\,4\)
#Fakes! Down and Up
root.exe -b -l -q syst.C+\(7\,5\,200\,8\,0\,4\)
root.exe -b -l -q syst.C+\(7\,5\,200\,8\,1\,4\)


root.exe -b -l -q datacardmaker7.C+\(200\,4\)


