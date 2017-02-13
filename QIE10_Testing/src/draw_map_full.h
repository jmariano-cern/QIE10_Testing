#include "TCanvas.h"
#include "TBox.h"
#include "TLatex.h"

static const int HF_num = 8;
static const int SL_num = 14;
static const int QI_num = 24;

static const int padMap[2][8] = { { 5,1,2,6,10,14,13,9 },{ 8,4,3,7,11,15,16,12 }};
				   

int**** create_error_map(){
  int**** lv2_err_map = new int***[2];
  for (int side=0; side<2; side ++){
    lv2_err_map[side] = new int**[HF_num];
    for (int hf=0; hf<HF_num ; hf++) {
      lv2_err_map[side][hf] = new int*[SL_num];
      for (int sl=0 ; sl<SL_num ; sl++) {
        lv2_err_map[side][hf][sl] = new int[QI_num];
      }
    }
  }
  for (int side=0; side<2; side++) {
    for (int hf=0; hf<HF_num ; hf++) {
      for (int sl=0 ; sl<SL_num ; sl++) {
        for (int qi=0 ; qi<QI_num ; qi++) {
          lv2_err_map[side][hf][sl][qi] = -1;
          // -1 = disabled
	  //  0 = fail
          //  1 = pass
        }
      }
    }
  }
  return lv2_err_map;
}

void draw_map(int**** lv2_err_map, int run_num, char const* folder_name, char const* title){

  /*
  int HF_num_adj = 0;
  for (int hf=0; hf<HF_num ; hf++) {
    if (lv0_mask[hf] == 1) {
      HF_num_adj++;
    }
  }
  */

  int HF_num_adj = 1;

  float HF_width = 0.97;
  float HF_w_buffer = (1-HF_width)/2;
  float HF_w_lower = 0 + HF_w_buffer;
  float HF_w_upper = 1 - HF_w_buffer;
  float HF_h = 0.8;
  float HF_h_lower = 0.05;
  float HF_h_unit = 1/((4*(float)HF_num_adj)+((float)HF_num_adj+1));
  float SL_width = HF_width/(float)SL_num;
  //float QI_height =  (4*HF_h_unit)/(float)QI_num;
  float QI_height =  (HF_h)/(float)QI_num;

  TCanvas *c1;
  TPad *pad1;
  TPad *pad2;

  TBox *HF;
  TBox *SL;
  TBox *QI;
  TBox *Quadrant;

  TLatex *text = new TLatex;
  char temp_text[512];

  char figure_name[512];
  char side_label[16];

  int hf = 0;

  bool crate_flag[2][HF_num] = {};
  bool temp_flag = 0; 
  for (int side=0; side<2; side++) {
    for (int hf_i=0 ; hf_i<HF_num ; hf_i++) {
      temp_flag = 0;
      for (int sl=0 ; sl<SL_num ; sl++) {
  	for (int qi=0 ; qi<QI_num ; qi++) {
          if(lv2_err_map[side][hf_i][sl][qi] != -1) {
            temp_flag = 1;
          }                    
        }  
      }
      crate_flag[side][hf_i] = temp_flag;
    }
  }

  c1 = new TCanvas("c1","c1",2000,1600);

  pad1 = new TPad("title","title",0,.9,1.,1.);
  pad2 = new TPad("pad","pad",0,0,1.,.9);
  pad1->Draw();
  pad2->Draw();

  pad2->Divide(4,4);
  for (int side =0; side<2; side++) {
    if (side == 0) {
      sprintf(side_label,"%s","M");
    } else {
      sprintf(side_label,"%s","P");
    }
    for (int hf_i=0 ; hf_i<HF_num ; hf_i++) {

      pad2->cd(padMap[side][hf_i]);
      
      if (crate_flag[side][hf_i] == 1) {

      
        for (int sl=0 ; sl<SL_num ; sl++) {
	
  	  for (int qi=0 ; qi<QI_num ; qi++) {
	    QI = new TBox( HF_w_lower+(sl*SL_width) , (HF_h+HF_h_lower) - (qi*QI_height) , HF_w_lower+((sl+1)*SL_width) , (HF_h+HF_h_lower) - ((qi+1)*QI_height) );
	    //	    QI = new TBox( HF_w_lower+(sl*SL_width) , (1-(((5*(1))-4)*HF_h_unit)) - (qi*QI_height) , HF_w_lower+((sl+1)*SL_width) , (1-(((5*(1))-4)*HF_h_unit)) - ((qi+1)*QI_height) );
	    if (lv2_err_map[side][hf_i][sl][qi] == 0) {
	      QI->SetFillColor(2);
	    } else if (lv2_err_map[side][hf_i][sl][qi] == 1){
	      QI->SetFillColor(3);
	    } else {
	      QI->SetFillColor(4);
	    }
	    QI->Draw();
	    sprintf(temp_text,"%i",qi+1);
	    text->SetTextAlign(23);
	    text->SetTextSize(0.7*QI_height);
	    //text->DrawLatex(HF_w_lower+((sl+0.4)*SL_width),(((5*(hf+1))-4)*HF_h_unit)+((qi+0.3)*QI_height),temp_text);
	    //	    text->DrawLatex( HF_w_lower+((sl+0.3)*SL_width) , (1-(((5*(1))-4)*HF_h_unit)) - ((qi+0.45)*QI_height) , temp_text );
	    text->DrawLatex( HF_w_lower+((sl+0.5)*SL_width) , (HF_h+HF_h_lower) - ((qi+0.45)*QI_height) , temp_text );

	  }
	
  	  //SL = new TBox(HF_w_lower+(sl*SL_width),((5*(hf+1))-4)*HF_h_unit,HF_w_lower+((sl+1)*SL_width),(5*(hf+1))*HF_h_unit);
	  SL = new TBox(HF_w_lower+(sl*SL_width),HF_h_lower,HF_w_lower+((sl+1)*SL_width),HF_h_lower+HF_h);
	  SL->SetFillStyle(0);
	  SL->SetLineColor(1);
	  SL->SetLineStyle(2);
	  SL->SetLineWidth(1);
	  SL->Draw();
	  sprintf(temp_text,"SL%i",sl+1);
	  text->SetTextAlign(23);
	  text->SetTextSize(SL_width/4);
	  //text->DrawLatex(HF_w_lower+((sl+0.2)*SL_width),((5*(hf+1))-4.2)*HF_h_unit,temp_text);
	  text->DrawLatex(HF_w_lower+((sl+0.5)*SL_width),0.8*HF_h_lower,temp_text);
        }
      
        //HF = new TBox(HF_w_lower,((5*(hf+1))-4)*HF_h_unit,HF_w_upper,(5*(hf+1))*HF_h_unit);
        HF = new TBox(HF_w_lower,HF_h_lower,HF_w_upper,HF_h_lower+HF_h);
	//        HF = new TBox(HF_w_lower,HF_h_unit,HF_w_upper,5*HF_h_unit);
        HF->SetFillStyle(0);
        HF->SetLineColor(1);
        HF->SetLineStyle(1);
        HF->SetLineWidth(1);
        HF->Draw();
        sprintf(temp_text,"HF%s0%i",side_label,hf_i+1);
        text->SetTextAlign(23);
        text->SetTextSize(0.6*HF_h_unit);
        //text->DrawLatex(0.48,((5*(hf+1))-4.6)*HF_h_unit,temp_text);
	//        text->DrawLatex(0.37,((5*(1))-4.6)*HF_h_unit,temp_text);
        text->DrawLatex(0.5,.95,temp_text);
      
        hf++;

        /* sprintf(figure_name,"$QIE10ROOT/img/%i/%s/%s_HF%s0%i.png",run_num,folder_name,title,side_label,hf_i+1); */
        /* c1->SaveAs(figure_name); */
        /* c1->Close(); */

      }
    }
  }

  c1->cd();
  pad2->cd();

  text->SetTextAlign(23);
  text->SetTextSize(0.05);
  sprintf(temp_text,"Q1");
  text->DrawLatex(0.02,0.75,temp_text);
  text->DrawLatex(0.77,0.75,temp_text);


  sprintf(temp_text,"Q2");
  text->DrawLatex(0.27,0.75,temp_text);
  text->DrawLatex(0.52,0.75,temp_text);

  sprintf(temp_text,"Q3");
  text->DrawLatex(0.27,0.25,temp_text);
  text->DrawLatex(0.52,0.25,temp_text);

  sprintf(temp_text,"Q4");
  text->DrawLatex(0.02,0.25,temp_text);
  text->DrawLatex(0.77,0.25,temp_text);


  Quadrant = new TBox();
  Quadrant->SetFillStyle(0);
  Quadrant->SetLineColor(1);
  Quadrant->SetLineStyle(2);
  Quadrant->SetLineWidth(2);
  //  Quadrant->Draw();

  Quadrant->DrawBox(0,0,1,.5);
  //  Quadrant->DrawBox(0,0.5,1,1);
  Quadrant->DrawBox(0.25,0,.75,1);
  //Quadrant->DrawBox(0.75,0,1,1);

  Quadrant->SetLineWidth(8);
  Quadrant->DrawBox(0,0,.5,1);
  Quadrant->DrawBox(0.5,0,1,1);

  sprintf(temp_text,"HFP");
  sprintf(temp_text,"HFM");
  text->DrawLatex(0.25,0.995,temp_text);
  sprintf(temp_text,"HFP");
  text->DrawLatex(0.75,0.995,temp_text);

  pad1->cd();
  text->SetTextAlign(22);
  sprintf(temp_text,"%s",title);
  text->SetTextSize(0.75);
  text->DrawLatex(0.5,0.5,temp_text);



  string saveName = title;
  replace(saveName.begin(),saveName.end(), ' ', '_');
  sprintf(figure_name,"$QIE10ROOT/img/%i/%s/%s.png",run_num,folder_name,saveName.c_str());
  c1->SaveAs(figure_name);
  c1->Close();

}
