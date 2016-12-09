///////////////////////////////////////////////////////////
//                                                        /
//                See instructions at :                   /
//                                                        /
//  https://github.com/falves10/HF_Calibration_Unit_Test  /
//                                                        /
///////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <array>
#include <string>
#include <fstream>
#include "TLegend.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TPad.h"
#include "TF1.h"
#include "TGraph.h"


using namespace std;

void pulser_test() {
  gStyle->SetOptStat(0); 
  //int runs = {22173}; //card 7
  //int runs = {22171}; card 11
  //int runs = {22177}; //card 10
  //int runs = {22178}; //card 8
  //int runs = {22179}; //card 9
  //int runs = {22180}; //card 4
 // int runs = {22204}; //card 5
 // int runs = {22762}; //
  int runs = {24694};
  float means_A[26];
  float means_B[26];
  float means_AB[26];
  float RMS_A[26];
  float RMS_B[26];
  float RMS_AB[26]; 
  float ratio_RMS_mean_A[26]; 
  float ratio_RMS_mean_B[26];
  float ratio_RMS_mean_AB[26];
  float BV_A[26];
  float BV_B[26];
  float BV_AB[26];

//================== RMS(IC)/Mean(IC) vs Bias Voltage ==============================
       // Code from Joseph
                int index_A = 0;
                int index_B = 0; 
                int index_AB = 0;
     //====== LED A ============	
                char root_file_name[512];
 	        sprintf(root_file_name,"../../dat/QIE10testing_10000%i_11.root",runs);
	        TFile *_file0 = TFile::Open(root_file_name);

 	        Int_t fe = 17;
                Int_t s = 11;
                Int_t q = 1;

                char hist_name[512];
                sprintf(hist_name,"Qsum_vs_Vb_LEDA/Qsum_vs_Vb_LEDA_HF%i_Slot%i_QIE%i", fe,s,q);
			
		TH1F *h = new TH1F();
		h = (TH1F*)_file0->Get(hist_name);
                for(float pointA = 0; pointA < 2.6; pointA += 0.1){ 
                	
 			h->GetXaxis()->SetRangeUser(pointA-0.05,pointA+0.05);	

                	h->GetMean(2);
               	 	h->GetRMS(2);
                              
                        means_A[index_A] = h->GetMean(2);
                        //cout<<"\n Mean A: "<< means_A[index_A] << endl;
                        RMS_A[index_A] = h->GetRMS(2);
		        ratio_RMS_mean_A[index_A] = RMS_A[index_A]/means_A[index_A];
                        BV_A[index_A] =  pointA;
                        index_A += 1;                    
                }
              
         //======= LED B =============

                char root_file_name1[512];
                sprintf(root_file_name1,"../../dat/QIE10testing_10000%i_11.root",runs);
                TFile *_file01 = TFile::Open(root_file_name1);

                Int_t fe1 = 17;
                Int_t s1 = 11;
                Int_t q1 = 1;


                char hist_name1[512];
                sprintf(hist_name1,"Qsum_vs_Vb_LEDB/Qsum_vs_Vb_LEDB_HF%i_Slot%i_QIE%i", fe1,s1,q1);

                TH1F *histo = new TH1F(); 
                histo = (TH1F*)_file01->Get(hist_name1);
                for(float pointB = 0; pointB < 2.6; pointB += 0.1){        

                        histo->GetXaxis()->SetRangeUser(pointB-0.05,pointB+0.05);

                        histo->GetMean(2);
                        histo->GetRMS(2);
                        //cout<<"\n index B: " << index_B << endl;
                        means_B[index_B] = histo->GetMean(2);
                        //cout<<"\n Mean B: "<< means_B[index_B] << endl;
                        RMS_B[index_B] = histo->GetRMS(2);
                        ratio_RMS_mean_B[index_B] = RMS_B[index_B]/means_B[index_B];
                        //cout<<"\n ratio: "<< ratio_RMS_mean_B[index_B] << endl;
                        BV_B[index_B] =  pointB;
                        index_B += 1;        
                }


         //========== LEDAB ===========
 
                char root_file_name2[512];
                sprintf(root_file_name2,"../../dat/QIE10testing_10000%i_11.root",runs);
                TFile *_file02 = TFile::Open(root_file_name2);

                Int_t fe2 = 17;
                Int_t s2 = 11;
                Int_t q2 = 1;


                char hist_name2[512];
                sprintf(hist_name2,"Qsum_vs_Vb_LEDAB/Qsum_vs_Vb_LEDAB_HF%i_Slot%i_QIE%i", fe2,s2,q2);

                TH1F *hist = new TH1F();
                hist = (TH1F*)_file02->Get(hist_name2);
                for(float pointAB = 0; pointAB < 2.6; pointAB += 0.1){

                        hist->GetXaxis()->SetRangeUser(pointAB-0.05,pointAB+0.05);

                        hist->GetMean(2);
                        hist->GetRMS(2);
                        //cout<<"\n index AB: " << index_AB << endl;
                        means_AB[index_AB] = hist->GetMean(2);
                        //cout<<"\n Mean AB: "<< means_AB[index_AB] << endl;
                        RMS_AB[index_AB] = hist->GetRMS(2);
                        ratio_RMS_mean_AB[index_AB] = RMS_AB[index_AB]/means_AB[index_AB];
                        //cout<<"\n ratio AB: "<< ratio_RMS_mean_AB[index_AB] << endl;
                        BV_AB[index_AB] =  pointAB;
                        index_AB += 1;
                }





//LED A
   //TCanvas *c1 = new TCanvas("c1","RMS(IC)/Mean(IC) vs bias voltage",200,10,700,500);
   Double_t x[26], y[26];
   Int_t n = 26;
   for (Int_t i=0;i<n;i++) {
     x[i] = BV_A[i];
     y[i] = ratio_RMS_mean_A[i];
   }
   TGraph *gr = new TGraph(n,x,y);
   gr->SetMarkerStyle(20);
   gr->SetMarkerColor(kRed);
   //gr->Draw("AP");
   gr->GetYaxis()->SetRangeUser(0., 1.);
   gr->SetTitle("RMS(IC)/Mean(IC) vs Bias Voltage  - LED A");
   gr->GetYaxis()->SetTitle("RMS(IC)/Mean(IC)");
   gr->GetXaxis()->SetTitle("Bias Voltage [V]");

//LED B

  //TCanvas *c2 = new TCanvas("c2","RMS(IC)/Mean(IC) vs bias voltage",200,10,700,500);
   Double_t x1[26], y1[26];
   Int_t n1 = 26;
   for (Int_t ii=0;ii<n1;ii++) {
     x1[ii] = BV_B[ii];
     cout<<"\n BV: "<< x1[ii] << endl;
     y1[ii] = ratio_RMS_mean_B[ii];
     cout<<"\n ratio: "<< y1[ii] << endl;
   }
   TGraph *gr1 = new TGraph(n1,x1,y1);
   gr1->SetMarkerStyle(22);
   gr1->SetMarkerColor(kBlue);
   //gr1->Draw("AP");
   gr1->GetYaxis()->SetRangeUser(0., 1.);
   gr1->SetTitle("RMS(IC)/Mean(IC) vs Bias Voltage  - LED B");
   gr1->GetYaxis()->SetTitle("RMS(IC)/Mean(IC)");
   gr1->GetXaxis()->SetTitle("Bias Voltage [V]");

//LED AB

  //TCanvas *c3 = new TCanvas("c3","RMS(IC)/Mean(IC) vs bias voltage",200,10,700,500);
   Double_t x2[26], y2[26];
   Int_t n2 = 26;
   for (Int_t iii=0;iii<n2;iii++) {
     x2[iii] = BV_AB[iii];
     y2[iii] = ratio_RMS_mean_AB[iii];
   }
   TGraph *gr2 = new TGraph(n2,x2,y2);
   gr2->SetMarkerStyle(21);
   gr2->SetMarkerColor(kBlack);
   //gr2->Draw("AP");
   gr2->GetYaxis()->SetRangeUser(0., 1.);
   gr2->SetTitle("RMS(IC)/Mean(IC) vs Bias Voltage  - LED AB");
   gr2->GetYaxis()->SetTitle("RMS(IC)/Mean(IC)");
   gr2->GetXaxis()->SetTitle("Bias Voltage [V]");

  TCanvas *c4 = new TCanvas("c4","RMS(IC)/Mean(IC) vs bias voltage",200,10,700,500);
     gr->Draw("AP");
     gr->SetTitle("RMS(IC)/Mean(IC) vs Bias Voltage - LEDs");
     gr1->Draw("P");
     gr2->Draw("P");
     
   TLegend *legend = new TLegend(0.8,0.6,0.5,0.75);
   legend->SetTextSize(0.04);
   legend->AddEntry(gr,"LED A","p");
   legend->AddEntry( gr1,"LED B","p");
   legend->AddEntry( gr2,"LED AB","p");
   legend->Draw();

   c4->SaveAs("NEW_RMS_over_BiasVoltage.pdf");    


//==============   LED A + LED B / LED(AB) ==============

                //LED A
                 
                float mean_A[26];
                int Index_A = 0; 
            
                char root_file_name_A[512];
                sprintf(root_file_name_A,"../../dat/QIE10testing_10000%i_11.root",runs);
                TFile *_file0_A = TFile::Open(root_file_name_A);

                Int_t fe_a = 17;
                Int_t s_a = 11;
                Int_t q_a = 1;

                char hist_name_A[512];
                sprintf(hist_name_A,"Qsum_vs_Vb_LEDA/Qsum_vs_Vb_LEDA_HF%i_Slot%i_QIE%i", fe_a,s_a,q_a);

                TH1F *ha = new TH1F();
                ha = (TH1F*)_file0_A->Get(hist_name_A);
                for(float point_A = 0; point_A < 2.6; point_A += 0.1){

                        ha->GetXaxis()->SetRangeUser(point_A-0.05,point_A+0.05);

                        ha->GetMean(2);

                        mean_A[Index_A] = ha->GetMean(2);
                        //cout<<"\n Mean IC - A: "<< mean_A[Index_A] << endl;
                        BV_A[Index_A] =  point_A;
                        //cout<<"\n Bias Voltage: "<<  BV_A[Index_A] << endl;
                        Index_A += 1;
                }

           
      
                //LED B
                float mean_B[26];
                int Index_B = 0;

                char root_file_name_B[512];
                sprintf(root_file_name_B,"../../dat/QIE10testing_10000%i_11.root",runs);
                TFile *_file0_B = TFile::Open(root_file_name_B);

                Int_t fe_b = 17;
                Int_t s_b = 11;
                Int_t q_b = 1;

                char hist_name_B[512];
                sprintf(hist_name_B,"Qsum_vs_Vb_LEDB/Qsum_vs_Vb_LEDB_HF%i_Slot%i_QIE%i", fe_b,s_b,q_b);

                TH1F *hb = new TH1F();
                hb = (TH1F*)_file0_B->Get(hist_name_B);
                for(float point_B = 0; point_B < 2.6; point_B += 0.1){

                        hb->GetXaxis()->SetRangeUser(point_B-0.05,point_B+0.05);

                        hb->GetMean(2);

                        mean_B[Index_B] = hb->GetMean(2);
                        //cout<<"\n Mean IC - B: "<< mean_B[Index_B] << endl;
                        BV_B[Index_B] =  point_B;
                        //cout<<"\n Bias Voltage: "<<  BV_B[Index_B] << endl;
                        Index_B += 1;
                }
 
              //LEDAB
                float mean_AB[26];
                int Index_AB = 0;

                char root_file_name_AB[512];
                sprintf(root_file_name_AB,"../../dat/QIE10testing_10000%i_11.root",runs);
                TFile *_file0_AB = TFile::Open(root_file_name_AB);

                Int_t fe_ab = 17;
                Int_t s_ab = 11;
                Int_t q_ab = 1;

                char hist_name_AB[512];
                sprintf(hist_name_AB,"Qsum_vs_Vb_LEDAB/Qsum_vs_Vb_LEDAB_HF%i_Slot%i_QIE%i", fe_ab,s_ab,q_ab);

                TH1F *hab = new TH1F();
                hab = (TH1F*)_file0_AB->Get(hist_name_AB);
                for(float point_AB = 0; point_AB < 2.6; point_AB += 0.1){

                        hab->GetXaxis()->SetRangeUser(point_AB-0.05,point_AB+0.05);

                        hab->GetMean(2);

                        mean_AB[Index_AB] = hab->GetMean(2);
                        //cout<<"\n Mean IC - AB: "<< mean_AB[Index_AB] << endl;
                        BV_AB[Index_AB] =  point_AB;
                        //cout<<"\n Bias Voltage: "<<  BV_AB[Index_AB] << endl;
                        Index_AB += 1;
                }
             

            //Calculate ratio
            float ratio[26];
            for( unsigned int it = 0; it  < (sizeof(mean_A)/sizeof(*mean_A)); it++){
                 //cout<<"\n Mean A: "<< mean_A[it] << endl;
                 //cout<<"\n Mean B: "<< mean_B[it] << endl;
                  //cout<<"\n Mean AB: "<< mean_AB[it] << endl;
                 ratio[it] = (mean_A[it] + mean_B[it])/ mean_AB[it];
                 //cout<<"\n Ratio - Linearity: "<< ratio[it] << endl;

            }
 

           TCanvas *c5 = new TCanvas("c5","Led A + Led B/ Led(AB)  vs bias voltage",200,10,700,500);
	   Double_t x_0[26], y_0[26];
  	   Int_t n_0 = 26;
   	   for (Int_t a = 0;a < n_0; a++) {
     		x_0[a] = BV_AB[a];
     		y_0[a] = ratio[a];
  	  }
  	 TGraph *gr_0 = new TGraph(n_0,x_0,y_0);
   	 gr_0->SetMarkerStyle(21);
   	 gr_0->SetMarkerColor(kBlack);
   	 gr_0->Draw("AP");
   	 //gr_0->GetYaxis()->SetRangeUser(0., 1.2);
   	 gr_0->SetTitle("Led A (IC) + Led B (IC) / Led(AB) (IC) vs Bias Voltage ");
   	 gr_0->GetYaxis()->SetTitle("[Led A (IC) + Led B (IC)]/Led(AB) (IC)");
   	 gr_0->GetXaxis()->SetTitle("Bias Voltage [V]");
        
         c5->SaveAs("NEW_Ratio_Linearity.pdf");     


   //========  INTEGRATED CHARGED VS BIAS VOLTAGE =======
                //LED A
                char root_file_name_LEDA[512];
                sprintf(root_file_name_LEDA,"../../dat/QIE10testing_10000%i_11.root",runs);
                TFile *_file0_LEDA = TFile::Open(root_file_name_LEDA);

                Int_t fe_A = 17;
                Int_t s_A = 11;
                Int_t q_A = 1;

                char hist_name_LEDA[512];
                sprintf(hist_name_LEDA,"Qsum_vs_Vb_LEDA/Qsum_vs_Vb_LEDA_HF%i_Slot%i_QIE%i", fe_A,s_A,q_A);
                TH1F *h_LEDA = new TH1F();
                h_LEDA = (TH1F*)_file0_LEDA->Get(hist_name_LEDA);
                //TCanvas *c6 = new TCanvas("c6","Integrated Charge  vs Bias voltage",200,10,700,500);
                h_LEDA->SetLineColor(kRed);
                //h_LEDA->Draw("BOX");
    
                //LED B
 
                char root_file_name_LEDB[512];
                sprintf(root_file_name_LEDB,"../../dat/QIE10testing_10000%i_11.root",runs);
                TFile *_file0_LEDB = TFile::Open(root_file_name_LEDB);

                Int_t fe_B = 17;
                Int_t s_B = 11;
                Int_t q_B = 1;

                char hist_name_LEDB[512];
                sprintf(hist_name_LEDB,"Qsum_vs_Vb_LEDB/Qsum_vs_Vb_LEDB_HF%i_Slot%i_QIE%i", fe_B,s_B,q_B);
                TH1F *h_LEDB = new TH1F();
                h_LEDB = (TH1F*)_file0_LEDB->Get(hist_name_LEDB);
                //TCanvas *c7 = new TCanvas("c7","Integrated Charge  vs Bias voltage",200,10,700,500);
                h_LEDB->SetLineColor(kBlue);
                //h_LEDB->Draw("BOX");

                // LED AB

                char root_file_name_LEDAB[512];
                sprintf(root_file_name_LEDAB,"../../dat/QIE10testing_10000%i_11.root",runs);
                TFile *_file0_LEDAB = TFile::Open(root_file_name_LEDAB);

                Int_t fe_AB = 17;
                Int_t s_AB = 11;
                Int_t q_AB = 1;

                char hist_name_LEDAB[512];
                sprintf(hist_name_LEDAB,"Qsum_vs_Vb_LEDAB/Qsum_vs_Vb_LEDAB_HF%i_Slot%i_QIE%i", fe_AB,s_AB,q_AB);
                TH1F *h_LEDAB = new TH1F();
                h_LEDAB = (TH1F*)_file0_LEDAB->Get(hist_name_LEDAB);
                //TCanvas *c8 = new TCanvas("c8","Integrated Charge  vs Bias voltage",200,10,700,500);
                h_LEDAB->SetLineColor(kBlack);
                //h_LEDAB->Draw("BOX");

                TCanvas *c9 = new TCanvas("c9","Integrated Charge  vs Bias voltage",200,10,700,500);
                 h_LEDA->Draw("BOX");
                 h_LEDA->SetTitle("Integrated Charge (IC) vs Bias Voltage");
                 h_LEDA->GetXaxis()->SetTitle("Bias Voltage [V] ");
                 h_LEDB->Draw("BOXSAME");               
                 h_LEDAB->Draw("BOXSAME");
           
                 TLegend *legendA = new TLegend(0.8,0.6,0.5,0.75);
		 legendA->SetTextSize(0.04);
   		 legendA->AddEntry(h_LEDA,"LED A","l");
   		 legendA->AddEntry(h_LEDB,"LED B","l");
   		 legendA->AddEntry(h_LEDAB,"LED AB","l");
   		 legendA->Draw();
              
                 c9->SaveAs("IC_vs_BiasVoltage_22798.pdf");     

     
}
	
//}	
