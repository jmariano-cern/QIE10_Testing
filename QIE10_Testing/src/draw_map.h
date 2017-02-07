#include "TCanvas.h"
#include "TBox.h"
#include "TLatex.h"

static const int HF_num = 8;
static const int SL_num = 14;
static const int QI_num = 24;

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

  float HF_width = 0.8;
  float HF_w_buffer = (1-HF_width)/2;
  float HF_w_lower = 0 + HF_w_buffer;
  float HF_w_upper = 1 - HF_w_buffer;
  float HF_h_unit = 1/((4*(float)HF_num_adj)+((float)HF_num_adj+1));
  float SL_width = HF_width/(float)SL_num;
  float QI_height =  (4*HF_h_unit)/(float)QI_num;

  TCanvas *c1;

  TBox *HF;
  TBox *SL;
  TBox *QI;

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

  for (int side =0; side<2; side++) {
    if (side == 0) {
      sprintf(side_label,"%s","M");
    } else {
      sprintf(side_label,"%s","P");
    }
    for (int hf_i=0 ; hf_i<HF_num ; hf_i++) {
      if (crate_flag[side][hf_i] == 1) {

        c1 = new TCanvas("c1","c1",800,1000);
        sprintf(temp_text,"%s",title);
        text->SetTextAlign(12);
        text->SetTextSize(0.07);
        text->DrawLatex(0.25,0.92,temp_text);
      
        for (int sl=0 ; sl<SL_num ; sl++) {
	
  	  for (int qi=0 ; qi<QI_num ; qi++) {
	    //QI = new TBox(HF_w_lower+(sl*SL_width),(((5*(hf+1))-4)*HF_h_unit)+(qi*QI_height),HF_w_lower+((sl+1)*SL_width),(((5*(hf+1))-4)*HF_h_unit)+((qi+1)*QI_height));
	    QI = new TBox( HF_w_lower+(sl*SL_width) , (1-(((5*(1))-4)*HF_h_unit)) - (qi*QI_height) , HF_w_lower+((sl+1)*SL_width) , (1-(((5*(1))-4)*HF_h_unit)) - ((qi+1)*QI_height) );
	    if (lv2_err_map[side][hf_i][sl][qi] == 0) {
	      QI->SetFillColor(2);
	    } else if (lv2_err_map[side][hf_i][sl][qi] == 1){
	      QI->SetFillColor(3);
	    } else {
	      QI->SetFillColor(4);
	    }
	    QI->Draw();
	    sprintf(temp_text,"%i",qi+1);
	    text->SetTextAlign(12);
	    text->SetTextSize(0.7*QI_height);
	    //text->DrawLatex(HF_w_lower+((sl+0.4)*SL_width),(((5*(hf+1))-4)*HF_h_unit)+((qi+0.3)*QI_height),temp_text);
	    text->DrawLatex( HF_w_lower+((sl+0.3)*SL_width) , (1-(((5*(1))-4)*HF_h_unit)) - ((qi+0.45)*QI_height) , temp_text );

	  }
	
  	  //SL = new TBox(HF_w_lower+(sl*SL_width),((5*(hf+1))-4)*HF_h_unit,HF_w_lower+((sl+1)*SL_width),(5*(hf+1))*HF_h_unit);
	  SL = new TBox(HF_w_lower+(sl*SL_width),((5*(1))-4)*HF_h_unit,HF_w_lower+((sl+1)*SL_width),(5*(1))*HF_h_unit);
	  SL->SetFillStyle(0);
	  SL->SetLineColor(1);
	  SL->SetLineStyle(2);
	  SL->SetLineWidth(1);
	  SL->Draw();
	  sprintf(temp_text,"SL%i",sl+1);
	  text->SetTextAlign(12);
	  text->SetTextSize(SL_width/4);
	  //text->DrawLatex(HF_w_lower+((sl+0.2)*SL_width),((5*(hf+1))-4.2)*HF_h_unit,temp_text);
	  text->DrawLatex(HF_w_lower+((sl+0.2)*SL_width),((5*(1))-4.2)*HF_h_unit,temp_text);
        }
      
        //HF = new TBox(HF_w_lower,((5*(hf+1))-4)*HF_h_unit,HF_w_upper,(5*(hf+1))*HF_h_unit);
        HF = new TBox(HF_w_lower,((5*(1))-4)*HF_h_unit,HF_w_upper,(5*(1))*HF_h_unit);
        HF->SetFillStyle(0);
        HF->SetLineColor(1);
        HF->SetLineStyle(1);
        HF->SetLineWidth(1);
        HF->Draw();
        sprintf(temp_text,"HF%s0%i",side_label,hf_i+1);
        text->SetTextAlign(12);
        text->SetTextSize(0.6*HF_h_unit);
        //text->DrawLatex(0.48,((5*(hf+1))-4.6)*HF_h_unit,temp_text);
        text->DrawLatex(0.37,((5*(1))-4.6)*HF_h_unit,temp_text);
      
        hf++;

        sprintf(figure_name,"$QIE10ROOT/img/%i/%s/%s_HF%i.png",run_num,folder_name,title,hf_i+1);
        c1->SaveAs(figure_name);
        c1->Close();

      }
    }
  }
}
