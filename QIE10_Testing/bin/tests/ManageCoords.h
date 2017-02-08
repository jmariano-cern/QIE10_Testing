int DET_ieta   = -999;
int DET_ieta_signed = -999;
int DET_iphi   = -999;
int DET_depth  = -999;
int FE_crate   = -999;
int FE_slot    = -999;
int FE_channel = -999;
int side       = -999;


void setCoords(int i) {
  DET_ieta  = activeChannels[i][0];                                                                                                          
  DET_iphi  = activeChannels[i][1];                                                                                                          
  DET_depth = activeChannels[i][2];                                                                                                          
  DET_ieta_signed = DET_ieta;                                                                                                                
  if (DET_ieta>0) {                                                                                                                          
    side = 1;                                                                                                                                
   } else {                                                                                                                                   
    side = 0;                                                                                                                                
    DET_ieta *= -1;                                                                                                                          
   }                                                                                                                                          
  FE_crate   = DETcoords2FEcoords[side][DET_ieta-29][(DET_iphi-1)/2][DET_depth-1][0];                                                        
  FE_slot    = DETcoords2FEcoords[side][DET_ieta-29][(DET_iphi-1)/2][DET_depth-1][1];                                                        
  FE_channel = DETcoords2FEcoords[side][DET_ieta-29][(DET_iphi-1)/2][DET_depth-1][2];
}
