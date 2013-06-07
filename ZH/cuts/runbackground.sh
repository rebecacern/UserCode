#run backgrounds at once
#do the WZ auxiliar rootfile
root.exe -b -l -q doWZ.C+\(8\,4\)

#and the WJets
root.exe -b -l -q doFakes.C+\(8\,4\)

root.exe -b -l -q chain.C+\(8\,0\,125\,4\)
root.exe -b -l -q chain.C+\(8\,2\,125\,4\)
root.exe -b -l -q chain.C+\(8\,3\,125\,4\)
root.exe -b -l -q chain.C+\(8\,4\,125\,4\)
root.exe -b -l -q chain.C+\(8\,5\,125\,4\)
#systematics, Stat Down
root.exe -b -l -q syst.C+\(8\,2\,125\,1\,0\,4\)
root.exe -b -l -q syst.C+\(8\,3\,125\,1\,0\,4\)
root.exe -b -l -q syst.C+\(8\,4\,125\,1\,0\,4\)
root.exe -b -l -q syst.C+\(8\,5\,125\,1\,0\,4\)
#Stat Up
root.exe -b -l -q syst.C+\(8\,2\,125\,1\,1\,4\)
root.exe -b -l -q syst.C+\(8\,3\,125\,1\,1\,4\)
root.exe -b -l -q syst.C+\(8\,4\,125\,1\,1\,4\)
root.exe -b -l -q syst.C+\(8\,5\,125\,1\,1\,4\)
#systematics, JES Down
root.exe -b -l -q syst.C+\(8\,2\,125\,2\,0\,4\)
root.exe -b -l -q syst.C+\(8\,3\,125\,2\,0\,4\)
root.exe -b -l -q syst.C+\(8\,4\,125\,2\,0\,4\)
#JES Up
root.exe -b -l -q syst.C+\(8\,2\,125\,2\,1\,4\)
root.exe -b -l -q syst.C+\(8\,3\,125\,2\,1\,4\)
root.exe -b -l -q syst.C+\(8\,4\,125\,2\,1\,4\)
#systematics, MET Down
root.exe -b -l -q syst.C+\(8\,2\,125\,4\,0\,4\)
root.exe -b -l -q syst.C+\(8\,3\,125\,4\,0\,4\)
root.exe -b -l -q syst.C+\(8\,4\,125\,4\,0\,4\)
#MET Up
root.exe -b -l -q syst.C+\(8\,2\,125\,4\,1\,4\)
root.exe -b -l -q syst.C+\(8\,3\,125\,4\,1\,4\)
root.exe -b -l -q syst.C+\(8\,4\,125\,4\,1\,4\)
#systematics, Lepton Down
root.exe -b -l -q syst.C+\(8\,2\,125\,5\,0\,4\)
root.exe -b -l -q syst.C+\(8\,3\,125\,5\,0\,4\)
root.exe -b -l -q syst.C+\(8\,4\,125\,5\,0\,4\)
#Lepton Up
root.exe -b -l -q syst.C+\(8\,2\,125\,5\,1\,4\)
root.exe -b -l -q syst.C+\(8\,3\,125\,5\,1\,4\)
root.exe -b -l -q syst.C+\(8\,4\,125\,5\,1\,4\)
#systematics, Lepton Eff Down
root.exe -b -l -q syst.C+\(8\,2\,125\,6\,0\,4\)
root.exe -b -l -q syst.C+\(8\,3\,125\,6\,0\,4\)
root.exe -b -l -q syst.C+\(8\,4\,125\,6\,0\,4\)
#Lepton Eff Up
root.exe -b -l -q syst.C+\(8\,2\,125\,6\,1\,4\)
root.exe -b -l -q syst.C+\(8\,3\,125\,6\,1\,4\)
root.exe -b -l -q syst.C+\(8\,4\,125\,6\,1\,4\)
#WZ Down and Up
root.exe -b -l -q syst.C+\(8\,2\,125\,7\,0\,4\)
root.exe -b -l -q syst.C+\(8\,2\,125\,7\,1\,4\)
#Fakes! Down and Up
root.exe -b -l -q syst.C+\(8\,5\,125\,8\,0\,4\)
root.exe -b -l -q syst.C+\(8\,5\,125\,8\,1\,4\)



#do the WZ auxiliar rootfile
root.exe -b -l -q doWZ.C+\(7\,4\)

#and the WJets
root.exe -b -l -q doFakes.C+\(7\,4\)

root.exe -b -l -q chain.C+\(7\,0\,125\,4\)
root.exe -b -l -q chain.C+\(7\,2\,125\,4\)
root.exe -b -l -q chain.C+\(7\,3\,125\,4\)
root.exe -b -l -q chain.C+\(7\,5\,125\,4\)
#systematics, Stat Down
root.exe -b -l -q syst.C+\(7\,2\,125\,1\,0\,4\)
root.exe -b -l -q syst.C+\(7\,3\,125\,1\,0\,4\)
root.exe -b -l -q syst.C+\(7\,5\,125\,1\,0\,4\)
#Stat Up
root.exe -b -l -q syst.C+\(7\,2\,125\,1\,1\,4\)
root.exe -b -l -q syst.C+\(7\,3\,125\,1\,1\,4\)
root.exe -b -l -q syst.C+\(7\,5\,125\,1\,1\,4\)
#systematics, JES Down
root.exe -b -l -q syst.C+\(7\,2\,125\,2\,0\,4\)
root.exe -b -l -q syst.C+\(7\,3\,125\,2\,0\,4\)
#JES Up
root.exe -b -l -q syst.C+\(7\,2\,125\,2\,1\,4\)
root.exe -b -l -q syst.C+\(7\,3\,125\,2\,1\,4\)
#systematics, MET Down
root.exe -b -l -q syst.C+\(7\,2\,125\,4\,0\,4\)
root.exe -b -l -q syst.C+\(7\,3\,125\,4\,0\,4\)
root.exe -b -l -q syst.C+\(7\,4\,125\,4\,0\,4\)
#MET Up
root.exe -b -l -q syst.C+\(7\,2\,125\,4\,1\,4\)
root.exe -b -l -q syst.C+\(7\,3\,125\,4\,1\,4\)
root.exe -b -l -q syst.C+\(7\,4\,125\,4\,1\,4\)
#systematics, Lepton Down
root.exe -b -l -q syst.C+\(7\,2\,125\,5\,0\,4\)
root.exe -b -l -q syst.C+\(7\,3\,125\,5\,0\,4\)
root.exe -b -l -q syst.C+\(7\,4\,125\,5\,0\,4\)
#Lepton Up
root.exe -b -l -q syst.C+\(7\,2\,125\,5\,1\,4\)
root.exe -b -l -q syst.C+\(7\,3\,125\,5\,1\,4\)
root.exe -b -l -q syst.C+\(7\,4\,125\,5\,1\,4\)

#systematics, Lepton Eff Down
root.exe -b -l -q syst.C+\(7\,2\,125\,6\,0\,4\)
root.exe -b -l -q syst.C+\(7\,3\,125\,6\,0\,4\)
#Lepton Eff Up
root.exe -b -l -q syst.C+\(7\,2\,125\,6\,1\,4\)
root.exe -b -l -q syst.C+\(7\,3\,125\,6\,1\,4\)
#WZ Down and Up
root.exe -b -l -q syst.C+\(7\,2\,125\,7\,0\,4\)
root.exe -b -l -q syst.C+\(7\,2\,125\,7\,1\,4\)
#Fakes! Down and Up
root.exe -b -l -q syst.C+\(7\,5\,125\,8\,0\,4\)
root.exe -b -l -q syst.C+\(7\,5\,125\,8\,1\,4\)

cp 125/* 110/.
cp 125/* 115/.
cp 125/* 120/.
cp 125/* 130/.
cp 125/* 135/.
cp 125/* 140/.
cp 125/* 150/.
cp 125/* 160/.
cp 125/* 170/.
cp 125/* 180/.
cp 125/* 190/.
cp 125/* 200/.
cp 125/* aux/.
