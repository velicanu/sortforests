// #include "/net/hisrv0001/home/dav2105/run/CMSSW_5_2_5_patch1/src/UserCode/CmsHi/HiForest/V2/hiForest_charge.h"
#include "HiForestAnalysis/hiForest.h"
#include <TFile.h>
#include <TH1D.h>
#include <TNtuple.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

vector< pair<int,double> > evtCentVz;
int comparecentvz (const void * a, const void * b);

void sortForest(int startline = 0, string flist = "")
{
  //! Define the input and output file and HiForest
  string buffer;
  vector<string> listoffiles;
  int nlines = 0;
  ifstream infile(flist.data());

  if (!infile.is_open()) {
    cout << "Error opening file. Exiting." << endl;
    return;
  } else {
    while (!infile.eof()) {
      infile >> buffer;
      listoffiles.push_back(buffer);
      nlines++;
    }
  }
  cout<<" here"<<endl;
  // HiForest *c = new HiForest(listoffiles[startline].data(),0,0,0,0,true);
  HiForest *c = new HiForest(listoffiles[startline].data(),"forest",cPPb);
  // TFile * outf = new TFile(Form("sortedHiForest_%d.root",startline),"recreate");
  // c->outf = outf;
  // c->SetOutputFile("null",true);
  c->SetOutputFile(Form("sortedHiForest_%d.root",startline));
  c->tree->SetMaxTreeSize(4000000000);

  c->LoadNoTrees();
  c->hasEvtTree = true;
  //! loop through all the events once to construct the cent,vz pair array we'll be sorting over
  cout << "Constructing the cent:vz pair array..." << endl;
  for (int i=0;i<c->GetEntries();i++)
  {
    c->GetEntry(i);
    pair<int,double> centvz;
    centvz.first = c->evt.hiBin;
    centvz.second = c->evt.vz;
    evtCentVz.push_back(centvz);
    if (i%1000==0) cout <<i<<" / "<<c->GetEntries()<<" "<<c->setupOutput<<endl;
  }
  c->ResetBooleans();
  //! Make the index array which will get sorted on first centrality
  int evtindecies[c->GetEntries()];
  for (int i=0;i<c->GetEntries();i++)
  {
    evtindecies[i] = i;
  }
  
  cout << "Sorting the cent:vz pair array..." << " "<<c->setupOutput<<endl;
  //! Sort the index array first on the centrality bin, then on the vz of the entry at that index
  qsort (evtindecies, c->GetEntries(), sizeof(int), comparecentvz);
  c->GoCrazy();
  //! Now fill the tree in the new order
  cout << "Filling the tree in the sorted order..." << " "<<c->setupOutput<<endl;
  for (int i=0;i<c->GetEntries();i++)
  {
    c->GetEntry(evtindecies[i]);
    c->FillOutput();
    if (i%1000==0) cout <<i<<" / "<<c->GetEntries()<<" "<<c->setupOutput<<endl;
  }

  delete c;
}

//! The comparison function for qsort which compares two indecies by looking up their centbin and vz in
//! evtCentVz pair array
int comparecentvz (const void * a, const void * b)
{
  if( (evtCentVz[*(int*)a].second - evtCentVz[*(int*)b].second) < 0 )
    return -1;
  else if( (evtCentVz[*(int*)a].second - evtCentVz[*(int*)b].second) > 0 )
    return 1;
  // if( (evtCentVz[*(int*)a].second - evtCentVz[*(int*)b].second) == 0 )
  return 0;
}

int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    std::cout << "Usage: sortForest <file_number> <file_list>" << std::endl;
    return 1;
  }
  sortForest(std::atoi(argv[1]), argv[2]);
  return 0;
}

