//FastSimulation headers
#include "FastSimulation/Calorimetry/interface/HCALResponse.h"
#include "FastSimulation/Utilities/interface/RandomEngine.h"

//CMSSW headers
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include<iostream>
#include <math.h>

#define debug 0

using namespace std;
using namespace edm;

HCALResponse::HCALResponse(const edm::ParameterSet& pset,
			   const RandomEngine* engine) :
  random(engine)
{ 
  
  RespPar[HCAL][0][0] =  
    pset.getParameter<double>("HadronBarrelResolution_Stochastic");
  RespPar[HCAL][0][1] =  
    pset.getParameter<double>("HadronBarrelResolution_Constant");
  RespPar[HCAL][0][2] =  
    pset.getParameter<double>("HadronBarrelResolution_Noise");

  RespPar[HCAL][1][0] =  
    pset.getParameter<double>("HadronEndcapResolution_Stochastic");
  RespPar[HCAL][1][1] =  
    pset.getParameter<double>("HadronEndcapResolution_Constant");
  RespPar[HCAL][1][2] =  
    pset.getParameter<double>("HadronEndcapResolution_Noise");

  RespPar[VFCAL][0][0] =  
    pset.getParameter<double>("HadronForwardResolution_Stochastic");
  RespPar[VFCAL][0][1] =  
    pset.getParameter<double>("HadronForwardResolution_Constant");
  RespPar[VFCAL][0][2] =  
    pset.getParameter<double>("HadronForwardResolution_Noise");

  RespPar[VFCAL][1][0] =  
    pset.getParameter<double>("ElectronForwardResolution_Stochastic");
  RespPar[VFCAL][1][1] =  
    pset.getParameter<double>("ElectronForwardResolution_Constant");
  RespPar[VFCAL][1][2] =  
    pset.getParameter<double>("ElectronForwardResolution_Noise");

  eResponseScale[0] = 
    pset.getParameter<double>("eResponseScaleHB");  
  eResponseScale[1] = 
    pset.getParameter<double>("eResponseScaleHE");
  eResponseScale[2] = 
    pset.getParameter<double>("eResponseScaleHF");

  eResponsePlateau[0] = 
    pset.getParameter<double>("eResponsePlateauHB");
  eResponsePlateau[1] = 
    pset.getParameter<double>("eResponsePlateauHE");
  eResponsePlateau[2] = 
    pset.getParameter<double>("eResponsePlateauHF");

  eResponseExponent = 
    pset.getParameter<double>("eResponseExponent");

  eResponseCoefficient = 
    pset.getParameter<double>("eResponseCoefficient");

  eResponseCorrection = 
    pset.getParameter<double>("eResponseCorrection");

  // If need - add a small energy to each hadron ...
  eBias = 
    pset.getParameter<double>("energyBias");
  
  
  etaStep = 0.1;
  muStep  = 0.25;

  double _etGridHD[maxHDet]  = {2., 5., 10., 30., 50., 100., 300.};
  double _eGridEM[maxEMe]    = {30., 100., 300., 1000., 3000.};
  double _eGridMU[maxMUet]  = {10., 30., 100., 300.};
  double _etaGridMU[maxMUeta] = {0.3, 0.6, 0.8, 1.25, 1.4, 3.0};

  // sometimes - too precise, just like it came from printout ...  

  double _meanHD[maxHDet][maxHDeta] = {

    {0.994468, 1.0262, 1.02414, 1.0349, 0.999161, 0.990296, 0.983258, 1.03589, 0.995821, 0.915769, 1.04892, 0.943691, 0.950034, 0.983885, 1.17969, 1.23256, 1.26104, 1.27074, 1.2294, 1.24782, 1.3176, 1.31863, 1.37126, 1.49193, 1.43893, 1.37383, 1.44897, 1.47199, 1.54021, 1.51138, 1.72481, 1.71607, 1.73071, 1.69458, 1.77888, 1.78425, 1.75436, 1.78725, 1.87764, 1.83765, 1.81408, 1.89184, 1.85836, 1.87247, 1.8646, 1.78507, 1.7724, 1.70668, 1.62427, 1.76511},
   
    {3.31527, 3.75306, 3.42369, 3.29881, 3.3948, 3.35145, 3.05204, 3.19147, 3.35079, 3.07687, 3.04298, 3.18721, 3.02148, 2.86283, 3.14803, 3.67982, 3.4493, 3.2266, 3.58878, 3.57067, 3.55242, 3.87631, 3.69709, 3.68992, 3.89424, 3.88816, 3.7645, 4.00871, 4.14604, 3.77017, 5.22469, 4.9671, 5.28952, 5.13815, 5.35394, 5.34329, 5.26225, 5.40104, 5.33468, 5.3519, 5.24724, 5.30481, 5.05893, 4.93604, 4.87402, 4.62679, 4.76472, 4.60116, 4.68426, 4.33045 },

    {7.85471, 7.763, 7.82678, 7.44299, 7.07115, 7.12614, 6.97074, 6.89456, 7.56452, 7.21237, 6.87805, 7.14484, 6.81336, 6.46097, 7.12894, 8.17613, 7.96138, 7.95148, 8.15345, 7.89831, 8.24539, 8.33444, 8.49381, 8.22022, 8.51541, 8.51572, 8.4223,8.40697, 8.61117, 8.80849, 10.759, 10.5922, 11.3817, 11.1463, 11.3032, 11.3331, 10.8891, 11.0871, 10.9791, 11.1042, 10.7969, 11.1019, 10.8262, 10.0955, 10.4024, 9.41198, 9.45667, 9.38563, 9.48742, 9.07623 },

    {27.7857, 27.4217, 26.939, 27.481, 26.3711, 25.6937, 25.8539, 25.3739, 25.2469, 24.7544, 24.9888, 24.7477, 23.9846, 22.553, 25.5412, 27.5875, 27.1883,   28.0176, 28.4269, 28.5451, 28.4682, 29.3502, 29.5558, 28.5768, 29.9074, 29.332, 29.1112, 28.4899,      28.4702, 30.054, 35.5651, 34.7344, 36.7311, 36.0769, 36.4205, 36.2927, 36.0602, 36.5526, 35.9435, 36.1103, 35.533, 35.5981, 34.744, 33.0079, 33.4431, 29.8716, 28.2019, 26.523, 28.4465, 28.2827 },

    {45.1645, 46.8512, 46.0512, 45.6828, 45.9842, 45.4282, 46.5366, 45.0095, 45.2007, 46.0511, 45.8358, 45.9931, 44.2881, 40.2625, 43.4337, 49.0706, 49.1706, 48.6588, 48.9443, 49.7949, 48.6816, 50.0434, 50.0783, 48.5773, 50.0774, 49.4599, 49.2705, 45.58, 47.706, 49.4067, 55.6453, 55.7636, 57.6321, 56.4545, 56.7876, 56.3377, 55.6879, 56.2179, 54.8244, 55.3925, 54.8871, 54.7371, 53.2685, 51.6678, 52.0687, 47.1896, 43.6761, 40.1869, 42.8487, 43.4443},

    {91.8549, 93.6403, 92.0097, 90.9477, 91.8475, 91.1624, 90.9411, 90.2465, 90.7695, 91.0897, 90.5224, 89.5086, 88.4556, 79.2406, 87.8602, 97.2937, 96.6274, 95.6939, 96.0179, 97.4769, 95.7802, 97.6862, 98.2862, 95.7245, 97.5253, 96.0755, 96.0739, 88.7817, 93.0197, 92.8896, 107.596, 107.96, 109.43, 107.894, 108.562, 108.224, 107.3, 108.26, 106.661, 106.498, 104.782, 105.515, 103.341, 99.9445, 99.6927, 91.1985, 83.5148, 73.4813, 81.6298, 84.4643 },

    {281.43, 284.334, 285.856, 284.353, 280.738, 283.031, 285.415, 281.793, 282.388, 286.562, 282.707, 281.216, 276.579, 248.453, 276.355, 298.996, 301.693, 297.476, 297.441, 296.045, 292.279, 298.971, 298.414, 289.472, 299.654, 293.939, 294.587, 269.31, 284.829, 214.658, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330, 330 }
  };

  double _sigmaHD[maxHDet][maxHDeta] = {

    {0.864071, 0.98447, 0.871354, 0.847881, 0.820159, 0.799855, 0.828699, 0.727436, 0.767031, 0.605875, 0.802254, 0.675848, 0.552111, 0.598287, 1.02585, 1.05914, 0.750607, 0.842944, 0.702275, 0.744423, 0.825636, 0.751682, 0.764432, 0.746703, 0.697447, 0.657776, 0.607216, 0.612901, 0.634544, 0.682171, 0.842804, 0.832992, 0.70721, 0.706302, 0.694817, 0.688575, 0.662423, 0.589267, 0.654093, 0.618051, 0.526045, 0.543144, 0.515212, 0.551783, 0.519453, 0.663107, 0.497675, 0.596644, 0.573251, 0.427364},
      
    {1.5713, 2.05589, 1.85726, 1.74273, 2.11737, 1.82561, 1.55446, 1.61983, 1.93853, 1.53942, 1.5435, 1.69262, 1.4317, 1.4282, 1.60709, 1.78467, 1.66988, 1.30054, 1.64779, 1.54459, 1.41142, 1.4541, 1.34559, 1.26003, 1.20173, 1.29206, 1.15527, 1.0512, 1.10316, 1.75615, 1.69583, 1.48187, 1.47732, 1.40701, 1.48612, 1.26917, 1.34044, 1.2543, 1.28569, 1.25219, 1.13289, 1.27917, 1.09344, 1.15541, 1.09778, 1.21229, 1.11381, 1.07572, 0.984415, 0.909833 },

    {3.18642, 3.29331, 3.09398, 2.85605, 2.82556, 2.89527, 2.6296, 2.83913, 2.93046, 2.72317, 2.62544, 2.76285, 2.4742, 2.62936, 2.70715, 2.72638, 2.65293, 2.33784, 2.46131, 2.36301, 2.27453, 2.03046, 2.20301, 2.00915, 1.75063, 1.75334, 1.66596, 1.60238, 1.80088, 3.18599, 2.48231, 2.29836, 2.19089, 2.3125, 2.11164, 1.75669, 2.1452, 2.04578, 2.06963, 1.87146, 2.08008, 2.03519, 2.00319, 2.28931, 2.22979, 2.55083, 2.4767, 2.44471, 1.89115, 1.55137 },

    {6.26116, 5.80487, 5.73928, 6.02186, 6.07223, 6.08565, 5.51642, 5.9126, 5.48196, 5.23463, 5.29874, 5.49789, 5.23809, 4.63073, 5.35075, 5.52779, 5.32913, 4.92123, 4.54429, 4.3257, 4.82104, 4.07188, 4.16744, 3.89421, 3.7165, 3.4778, 3.38921, 3.04246, 3.7106, 6.06222, 6.32303, 5.63618, 4.87904, 4.58962, 5.71972, 5.32256, 4.7164, 4.35502, 4.35531, 4.47801, 4.32158, 4.93618, 4.30219, 5.56533, 5.02983, 6.67571, 7.63136, 7.27706, 6.22843, 4.55151  },

    {7.27726, 8.10476, 7.37787, 8.05064, 8.01531, 7.86412, 8.35105, 8.05885, 7.94422, 7.94245, 7.99923, 8.17982, 7.53708, 7.00575, 8.27399, 7.52417, 6.84759, 6.57941, 6.41338, 5.90157, 5.87598, 6.35299, 5.2332, 5.19234, 5.55871, 4.69554, 4.57994, 3.85366, 5.74974, 11.8269, 9.11508, 8.33659, 6.76692, 6.46447, 7.15199, 7.47847, 6.93844, 7.35615, 7.31525, 7.73408, 5.40751, 6.72558, 7.03997, 7.47892, 6.58941, 9.66841, 12.3704, 12.5351, 11.839, 8.78713 },

    {12.7001, 13.5512, 12.7775, 12.4712, 13.3088, 12.8748, 12.8265, 12.9869, 12.9305, 13.2273, 13.8033, 13.9653, 12.177, 13.9365, 12.8924, 10.8366, 9.43519, 9.81594, 10.282, 10.958, 10.4934, 8.79936, 8.44153, 7.92623, 9.63206, 8.75755, 7.57909, 6.57533, 9.07104, 27.0614, 15.2334, 12.2836, 12.9684, 10.1321, 13.4777, 12.3912, 11.8814, 12.4256, 10.4787, 13.2936, 11.0627, 13.1273, 10.5516, 12.846, 11.8735, 17.7823, 23.8047, 26.6134, 22.7521, 13.7872},


    {31.8604, 25.9982, 30.2161, 29.428, 32.9713, 27.8436, 29.4797, 30.665, 33.7663, 28.0207, 31.4693, 32.7732, 31.8299, 32.7413, 27.3617, 25.0852, 22.1768, 23.798, 19.9974, 19.7066, 19.6296, 18.7529, 19.1481, 15.3744, 19.6767, 18.3403, 16.0186, 13.1443, 25.4105, 73.2063,  40,   40,   40,   40,   40,   40,   40,   40,   40,   40,   40,   40,   40,   40,   40,   40,   40,   40,   40, 40}
    };


  // e-gamma mean response and sigma in HF 
  double _meanEM[maxEMe][maxEMeta] = 
    { 
       {22.15, 19.86, 23.73, 21.18, 21.61, 21.44, 17.84, 20.84, 18.57, 20.82, 19.76, 19.18, 20.52, 17.52, 16.99, 13.24, 16.33, 13.96, 12.7, 13.73},

      {74.4, 71.07, 74.38, 72.97, 73.14, 72.28, 68.42, 70.15, 69.66, 70.99, 69.19, 68.18, 70.05, 63.38, 46.62, 37.35, 26.47, 17.66, 24.08, 45.16},

  {221.437, 221.368, 222.783, 221.25, 219.083, 209.88, 206.721, 212.441, 211.412, 209.134, 206.064, 209.892, 206.92, 199.872, 140.668, 113.588, 51.5992, 27.5073, 38.904, 125.717},

      {759.313, 778.377, 759.422, 766.935, 734.067, 706.534, 697.785, 697.526, 708.746, 706.14, 685.874, 683.319, 677.348, 656.552, 502.097, 418.544, 97.3635, 62.7007, 100.774, 462.385},
    
      {2303.92, 2297.72, 2308.41, 2336.72, 2236.88, 2144.91, 2060.43, 2129.89, 2100.12, 2126.81, 2089.92, 2087.01, 2098.24, 1960.21, 1559.07, 1354.58, 486.69, 98.7363, 185.586, 1452.44}
    }; 

  double _sigmaEM[maxEMe][maxEMeta] =
    { 
      {10.01, 9.868, 9.699, 8.164, 8.973, 8.622, 8.275, 8.99, 9.267, 8.84, 9.112, 8.645, 8.947, 8.261, 7.808, 6.609, 7.955, 8.72, 5.978, 5.634},

      {23.01, 19.89, 22.86, 22.72, 19.62, 21.02, 18.72, 20.61, 20.38, 20.56, 21.89, 21.32, 20.96, 22.2, 20.95, 19.13, 19.37, 10.99, 18.76, 18.18},

      {52.19, 51.31, 52.33, 52.59, 50.07, 44.66, 48.86, 47.47, 49.44, 45.16, 50.99, 49.63, 51.04,   58, 54.86,   64, 57.27, 31.94, 43.17, 46.62},

      {167.3,  197, 166.4, 171.9, 148.9, 143.7, 173.4,  135, 159.9, 166.8,  151, 145.2, 158.6, 181.5, 155.3, 213.8, 126.1, 88.07, 132.4, 172.8},

      {426.8, 433.6, 464.1, 465.6, 429.3, 393.3, 426.7, 406.1, 378.8, 442.9, 460.2, 426.8, 458.1, 505.6, 476.1, 514.4,  264., 215.6, 293.1, 471.3}

    }; 


  // MUON probability histos for bin size = 0.25 GeV (0-10 GeV, 40 bins)  

  double _responseMU[maxMUet][maxMUeta][maxMUbin] = {
   { 
  // 10 GeV
     {1, 0.999009, 0.999009, 0.999009, 0.997027, 0.981169, 0.949455, 0.922696, 0.863231, 0.782953, 0.665015, 0.555996, 0.443013, 0.322101, 0.230922, 0.162537, 0.114965, 0.0812686, 0.0604559, 0.0475719, 0.0376611, 0.0307235, 0.0178394, 0.0138751, 0.0099108, 0.00792864, 0.00792864, 0.00693756, 0.00396432, 0.00297324, 0.00297324, 0.00198216, 0.00198216, 0.00198216, 0.00198216, 0.00198216, 0.00198216, 0.00099108, 0.00099108, 0}, 
     {1, 1, 1, 0.998037, 0.993131, 0.976447, 0.908734, 0.843965, 0.698724, 0.546614, 0.437684, 0.336605, 0.245339, 0.177625, 0.123651, 0.077527, 0.063788, 0.0451423, 0.0353288, 0.0284593, 0.0245339, 0.0215898, 0.0186457, 0.0147203, 0.013739, 0.00981354, 0.00490677, 0.00490677, 0.00392542, 0.00294406, 0.00294406, 0.00196271, 0.00196271, 0.00196271, 0.00196271, 0.000981354, 0.000981354, 0.000981354, 0.000981354, 0}, 
   {1, 1, 1, 1, 0.99854, 0.986861, 0.964964, 0.941606, 0.870073, 0.747445, 0.640876, 0.562044, 0.426277, 0.344526, 0.255474, 0.191241, 0.135766, 0.109489, 0.0729927, 0.0525547, 0.0423358, 0.0291971, 0.0233577, 0.0175182, 0.0131387, 0.010219, 0.00437956, 0.00437956, 0.00437956, 0.00291971, 0.00145985, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 
   {1, 1, 1, 1, 1, 1, 0.987603, 0.982782, 0.9573, 0.909091, 0.85124, 0.800275, 0.727961, 0.626722, 0.536501, 0.447658, 0.341598, 0.274793, 0.208678, 0.153581, 0.125344, 0.0971074, 0.0764463, 0.0633609, 0.0495868, 0.0371901, 0.0316804, 0.0213499, 0.0172176, 0.0144628, 0.0110193, 0.00964187, 0.00826446, 0.00757576, 0.00688705, 0.00413223, 0.00344353, 0.00206612, 0.00206612, 0.00206612}, 
    {1, 0.995918, 0.995918, 0.995918, 0.953061, 0.846939, 0.75102, 0.634694, 0.512245, 0.355102, 0.263265, 0.218367, 0.136735, 0.0836735, 0.0612245, 0.0469388, 0.0326531, 0.0265306, 0.0142857, 0.0102041, 0.00408163, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0},
   {1, 0.999541, 0.999541, 0.999541, 0.996784, 0.977257, 0.977257, 0.909028, 0.909028, 0.769354, 0.698369, 0.698369, 0.455088, 0.244659, 0.244429, 0.129566, 0.129336, 0.0884448, 0.0443372, 0.0438778, 0.0238916, 0.0238916, 0.0130944, 0.00987824, 0.00735125, 0.00574317, 0.00436481, 0.00229727, 0.00229727, 0.00160809, 0.00160809, 0.00160809, 0.00114863, 0.00114863, 0.000459453, 0.000229727, 0.000229727, 0.000229727, 0.000229727, 0}
   },
   //30 GeV
   {
     {1, 1, 1, 1, 1, 0.997006, 0.981038, 0.963074, 0.928144, 0.852295, 0.761477, 0.662675, 0.538922, 0.399202, 0.299401, 0.234531, 0.190619, 0.154691, 0.125749, 0.0918164, 0.0718563, 0.0638723, 0.0568862, 0.0399202, 0.0319361, 0.0259481, 0.0199601, 0.0159681, 0.0149701, 0.011976, 0.00898204, 0.00798403, 0.00299401, 0.00299401, 0.00299401, 0.00299401, 0.00199601, 0.00199601, 0.000998004, 0.000998004}, 
     {1, 0.998024, 0.998024, 0.998024, 0.996047, 0.982213, 0.927866, 0.885375, 0.791502, 0.601779, 0.477273, 0.383399, 0.29249, 0.205534, 0.177866, 0.126482, 0.100791, 0.0790514, 0.0494071, 0.0375494, 0.0286561, 0.0217391, 0.0217391, 0.0158103, 0.0128458, 0.0118577, 0.0108696, 0.00988142, 0.00889328, 0.00889328, 0.00889328, 0.00790514, 0.00592885, 0.00592885, 0.00395257, 0.00197628, 0.000988142, 0.000988142, 0, 0}, 
     {1, 1, 1, 1, 1, 0.997041, 0.97929, 0.964497, 0.903846, 0.803254, 0.681953, 0.594675, 0.5, 0.360947, 0.298817, 0.214497, 0.150888, 0.130178, 0.0872781, 0.0695266, 0.0591716, 0.0384615, 0.0340237, 0.0251479, 0.0192308, 0.0177515, 0.0177515, 0.0162722, 0.0162722, 0.0133136, 0.010355, 0.00739645, 0.00443787, 0.00443787, 0.00443787, 0.00443787, 0.00295858, 0.00295858, 0, 0}, 
     {1, 1, 1, 1, 1, 0.999306, 0.997917, 0.997222, 0.986111, 0.953472, 0.901389, 0.865972, 0.775, 0.6625, 0.603472, 0.501389, 0.402083, 0.349306, 0.264583, 0.220833, 0.194444, 0.145139, 0.119444, 0.09375, 0.0763889, 0.0625, 0.0472222, 0.0368056, 0.0326389, 0.0291667, 0.0201389, 0.0145833, 0.0145833, 0.0111111, 0.00833333, 0.00694444, 0.00486111, 0.00416667, 0.00208333, 0.000694444}, 
     {1, 0.991803, 0.991803, 0.991803, 0.969262, 0.881148, 0.762295, 0.67623, 0.532787, 0.409836, 0.295082, 0.243852, 0.188525, 0.139344, 0.122951, 0.0778689, 0.0532787, 0.0471311, 0.0368852, 0.0286885, 0.0286885, 0.022541, 0.0184426, 0.0163934, 0.0163934, 0.0122951, 0.0122951, 0.00819672, 0.00819672, 0.00819672, 0.00819672, 0.00614754, 0.00204918, 0.00204918, 0.00204918, 0.00204918, 0, 0, 0, 0}, 
     {1, 0.999304, 0.999304, 0.999304, 0.998609, 0.990958, 0.990958, 0.952469, 0.952237, 0.819615, 0.777417, 0.777417, 0.563877, 0.332251, 0.332251, 0.190587, 0.190587, 0.157663, 0.0987712, 0.0987712, 0.061674, 0.061674, 0.0377927, 0.0352423, 0.0268954, 0.0257361, 0.0234176, 0.0132159, 0.0127521, 0.0106654, 0.0106654, 0.00881057, 0.00394157, 0.00394157, 0.00370971, 0.001623, 0.001623, 0.000927429, 0.000927429, 0.000695572}
   },
   // 100 GeV
   {
     {1, 1, 1, 1, 1, 0.998976, 0.9826, 0.971341, 0.947799, 0.895599, 0.804504, 0.73695, 0.650972, 0.520983, 0.417605, 0.350051, 0.286592, 0.239509, 0.192426, 0.146366, 0.125896, 0.106448, 0.0921187, 0.073695, 0.0593654, 0.0470829, 0.0388946, 0.0286592, 0.0225179, 0.0214944, 0.0184237, 0.0153531, 0.0143296, 0.011259, 0.00921187, 0.00716479, 0.00716479, 0.00511771, 0.00409417, 0.00307062}, 
    {1, 1, 1, 1, 0.999004, 0.989044, 0.949203, 0.922311, 0.850598, 0.724104, 0.600598, 0.510956, 0.432271, 0.334661, 0.293825, 0.217131, 0.184263, 0.164343, 0.123506, 0.10757, 0.0916335, 0.0717131, 0.062749, 0.0507968, 0.0418327, 0.0408367, 0.0328685, 0.0278884, 0.0268924, 0.0229084, 0.0199203, 0.0179283, 0.0149402, 0.0119522, 0.0109562, 0.00996016, 0.00896414, 0.00697211, 0.00498008, 0.00199203},
    {1, 1, 1, 1, 1, 0.994048, 0.985119, 0.974702, 0.934524, 0.827381, 0.708333, 0.639881, 0.5625, 0.450893, 0.392857, 0.316964, 0.258929, 0.235119, 0.162202, 0.130952, 0.120536, 0.0907738, 0.078869, 0.0669643, 0.0625, 0.0565476, 0.047619, 0.0357143, 0.0342262, 0.0282738, 0.0208333, 0.0208333, 0.016369, 0.0133929, 0.0119048, 0.0104167, 0.00446429, 0.00446429, 0.0014881, 0.0014881}, 
    {1, 0.999281, 0.999281, 0.999281, 0.999281, 0.999281, 0.997124, 0.996405, 0.991373, 0.966211, 0.925953, 0.893602, 0.839684, 0.742631, 0.695902, 0.597412, 0.507549, 0.470884, 0.38821, 0.327822, 0.295471, 0.230769, 0.199856, 0.166068, 0.136592, 0.119339, 0.0941769, 0.0754853, 0.069734, 0.0603882, 0.0510424, 0.0409777, 0.0359454, 0.0287563, 0.0201294, 0.0143781, 0.0107836, 0.00790798, 0.00647017, 0.00359454}, 
    {1, 0.997934, 0.997934, 0.997934, 0.987603, 0.92562, 0.834711, 0.77686, 0.654959, 0.518595, 0.409091, 0.353306, 0.299587, 0.216942, 0.206612, 0.161157, 0.130165, 0.123967, 0.0909091, 0.0867769, 0.0681818, 0.053719, 0.0433884, 0.035124, 0.0330579, 0.0330579, 0.0289256, 0.0268595, 0.0227273, 0.0227273, 0.0206612, 0.018595, 0.0123967, 0.0103306, 0.00619835, 0.00619835, 0.00413223, 0.00413223, 0.00206612, 0.00206612},
    {1, 1, 1, 1, 0.999765, 0.995535, 0.9953, 0.970153, 0.969918, 0.881316, 0.853584, 0.853584, 0.670035, 0.441833, 0.441833, 0.286251, 0.286016, 0.251234, 0.171798, 0.171798, 0.114454, 0.114454, 0.0777908, 0.0747356, 0.0571093, 0.0552291, 0.053349, 0.0293772, 0.0293772, 0.026557, 0.0249119, 0.0211516, 0.012691, 0.0115159, 0.0110458, 0.00658049, 0.00634548, 0.00305523, 0.00305523, 0.00188014}
   },
   // 300 GeV
   {
     {1, 1, 1, 1, 1, 0.997817, 0.99345, 0.983624, 0.966157, 0.933406, 0.873362, 0.815502, 0.74345, 0.64083, 0.558952, 0.479258, 0.419214, 0.379913, 0.325328, 0.270742, 0.239083, 0.19869, 0.167031, 0.141921, 0.113537, 0.0971616, 0.0840611, 0.0687773, 0.058952, 0.0436681, 0.0393013, 0.0327511, 0.0305677, 0.0262009, 0.0196507, 0.0163755, 0.0120087, 0.00982533, 0.00873362, 0.00327511}, 
    {1, 1, 1, 1, 1, 0.996865, 0.972832, 0.948798, 0.904911, 0.802508, 0.719958, 0.649948, 0.584117, 0.491118, 0.46604, 0.364681, 0.331243, 0.308255, 0.244514, 0.221526, 0.202717, 0.169279, 0.15256, 0.126437, 0.111808, 0.106583, 0.0909091, 0.0783699, 0.0710554, 0.0574713, 0.0501567, 0.045977, 0.0376176, 0.0344828, 0.030303, 0.0261233, 0.0188088, 0.0135841, 0.0104493, 0.00522466}, 
    {1, 1, 1, 1, 1, 0.998415, 0.993661, 0.988906, 0.977813, 0.935024, 0.858954, 0.812995, 0.762282, 0.662441, 0.622821, 0.505547, 0.445325, 0.426307, 0.329635, 0.29794, 0.275753, 0.242472, 0.22187, 0.191759, 0.171157, 0.158479, 0.141046, 0.125198, 0.120444, 0.110935, 0.0982567, 0.0792393, 0.0586371, 0.0570523, 0.0538827, 0.0459588, 0.0332805, 0.0269414, 0.0174326, 0.0110935}, 
    {1, 1, 1, 1, 1, 1, 0.998436, 0.997654, 0.992963, 0.982017, 0.960125, 0.94527, 0.906177, 0.845192, 0.820954, 0.744332, 0.671618, 0.624707, 0.538702, 0.475371, 0.441751, 0.36982, 0.333855, 0.291634, 0.250977, 0.22674, 0.199375, 0.1681, 0.150899, 0.137608, 0.110242, 0.0992963, 0.0820954, 0.0664582, 0.0578577, 0.0453479, 0.0359656, 0.0289289, 0.0187647, 0.00938233},
    {1, 1, 1, 1, 0.989177, 0.941558, 0.887446, 0.839827, 0.75974, 0.612554, 0.525974, 0.478355, 0.426407, 0.361472, 0.350649, 0.279221, 0.253247, 0.24026, 0.168831, 0.145022, 0.134199, 0.108225, 0.101732, 0.0779221, 0.0606061, 0.0562771, 0.0497835, 0.04329, 0.0411255, 0.034632, 0.034632, 0.0238095, 0.0194805, 0.017316, 0.017316, 0.0108225, 0.0108225, 0.0108225, 0.00649351, 0.00649351},
     {1, 0.99926, 0.99926, 0.99926, 0.99852, 0.995312, 0.995312, 0.984949, 0.984703, 0.925241, 0.915865, 0.915865, 0.798668, 0.630891, 0.630891, 0.484826, 0.484579, 0.455712, 0.356279, 0.356279, 0.268937, 0.268937, 0.196891, 0.191463, 0.154453, 0.149519, 0.146558, 0.0957316, 0.0949914, 0.0885764, 0.0880829, 0.0745127, 0.0478658, 0.0471256, 0.0446583, 0.0264002, 0.0261535, 0.0157908, 0.0152973, 0.00986923}
   }
  };

  // now assign
  for (int i1 = 0; i1 < maxMUet; i1++) { 
    eGridMU[i1] = _eGridMU[i1];
    for (int i2 = 0; i2 < maxMUeta; i2++) {
      for (int i3 = 0; i3 < maxMUbin; i3++) {
	
	responseMU[i1][i2][i3] = _responseMU[i1][i2][i3];

	if(debug) {
	  //cout.width(6);
	  LogDebug("FastCalorimetry") << " responseMU " << i1 << " " << i2 << " " << i3  << " = " 
	       << responseMU[i1][i2][i3] << endl;
	}
	
      }
    }
  }
  for (int i2 = 0; i2 < maxMUeta; i2++) {
    etaGridMU[i2] = _etaGridMU[i2];
  }


  // Normalize the response and sigmas to the corresponding energies
  for(int i = 0; i<maxHDet;  i++) {
    etGridHD[i] = _etGridHD[i];
    for(int j = 0; j<maxHDeta; j++) {
       meanHD[i][j] =  _meanHD[i][j] / etGridHD[i];
      sigmaHD[i][j] =  _sigmaHD[i][j] / etGridHD[i];
    }
  }

  for(int i = 0; i<maxEMe;  i++) {
    eGridEM[i] = _eGridEM[i];
    for(int j = 0; j<maxEMeta; j++) {
       meanEM[i][j] = _meanEM[i][j] / eGridEM[i];
      sigmaEM[i][j] = _sigmaEM[i][j] / eGridEM[i];
    }
  }


}

 
pair<double,double> 
HCALResponse::responseHCAL(double energy, double eta, int partype)
{

  int ieta = abs((int)(eta / etaStep)) ;
  int ie = -1;

  // e/gamma
  if(partype == 0) {
    ieta -= 30; // HF starts at eta=3, while response vector from index = 0...
    if(ieta >= maxEMeta ) ieta = maxEMeta;
    else if(ieta < 0) ieta = 0;
 
    for (int i = 0; i < maxEMe; i++) {
      if(energy < eGridEM[i])  {
	if(i == 0) ie = 0;       
        else  ie = i-1;
        break;
      }
    }
    if(ie == -1) ie = maxEMe - 2;  
    interEM(energy, ie, ieta);
  }  
     
  else { 
    // hadrons
    if(partype == 1) {  
      
      if(ieta >= maxHDeta) ieta = maxHDeta;
      else if(ieta < 0) ieta = 0;
      
      double et = energy / cosh (eta);
      for (int i = 0; i < maxHDet; i++) {
	if(et < etGridHD[i])  {
	  if(i == 0) ie = 0;     // less than minimal -
	  else  ie = i-1;        // back extrapolation with the first interval
	  break;
	}
      }
      if(ie == -1) ie = maxHDet - 2;     // more than maximum - 
      interHD(et, ie, ieta);             // extrapolation using the last interv
    }
    
    // translate from Et normalized to energy and apply energy scale correction
    mean  *= cosh(eta) * eResponseCorrection;
    if(partype == 1) mean  += eBias;
    sigma *= cosh(eta) * eResponseCorrection;
  }
  
  // muons
  if(partype == 2) { 

    ieta = maxMUeta;
    for(int i = 0; i < maxMUeta; i++) {
      if(fabs(eta) < etaGridMU[i]) {
	ieta = i;  
	break;
      }       
    }     
    
    if(ieta < 0) ieta = 0;
    if(ieta < maxMUeta) {
      
	for (int i = 0; i < maxMUet; i++) {
	  if(energy < eGridMU[i])  {
	    if(i == 0) ie = 0;     // less than minimal -
	    else  ie = i-1;        // back extrapolation with the first interval
	    break;
	  }
	}
	if(ie == -1) ie = maxMUet - 2;     // more than maximum - 
	interMU(energy, ie, ieta);             // extrapolation using the last interv
        if(mean > energy) mean = energy;  

    }
    else mean = 0.;                      // muons in HF
  }


  // debugging
  if(debug) {
    //  cout.width(6);
  LogDebug("FastCalorimetry") << endl
       << " HCALResponse::responseHCAL, partype = " <<  partype << endl
       << " E, eta, part = " << energy << " " << eta << " " << partype << endl
       << " mean & sigma = " << mean   << " " << sigma << endl;

  }

  return pair<double,double>(mean,sigma);

}

void HCALResponse::interMU(double e, int ie, int ieta)
{

  double x = random->flatShoot();

  int bin1 = maxMUbin;
  for(int i = 0; i < maxMUbin; i++) {
    if(x > responseMU[ie][ieta][i]) {
      bin1 = i-1;
      break;
    }
  }
  int bin2 = maxMUbin;
  for(int i = 0; i < maxMUbin; i++) {
    if(x > responseMU[ie+1][ieta][i]) {
      bin2 = i-1;
      break;
    }
  }
   
  double x1 = eGridMU[ie];
  double x2 = eGridMU[ie+1];
  double y1 = (bin1 + random->flatShoot()) * muStep;   
  double y2 = (bin2 + random->flatShoot()) * muStep;   

  if(debug) {
    //  cout.width(6);
  LogDebug("FastCalorimetry") << endl
       << " HCALResponse::interMU  " << endl
       << " x, x1-x2, y1-y2 = " 
       << e << ", " << x1 <<"-" << x2 << " " << y1 <<"-" << y2 << endl; 
  
  }


  mean  = y1 + (y2-y1) * (e - x1)/(x2 - x1);
  sigma = 0.;

  if(debug) {
    //cout.width(6);
  LogDebug("FastCalorimetry") << endl
       << " HCALResponse::interMU " << endl
       << " e, ie, ieta = " << e << " " << ie << " " << ieta << endl
       << " response  = " << mean << endl; 
  }

}


void HCALResponse::interHD(double et, int ie, int ieta)
{
  double y1 = meanHD[ie][ieta]; 
  double y2 = meanHD[ie+1][ieta]; 
  double x1 = etGridHD[ie];
  double x2 = etGridHD[ie+1];

  if(debug) {
    //  cout.width(6);
  LogDebug("FastCalorimetry") << endl
       << " HCALResponse::interHD mean " << endl
       << " x, x1-x2, y1-y2 = " 
       << et << ", " << x1 <<"-" << x2 << " " << y1 <<"-" << y2 << endl; 
  
  }
  
  mean =  et * (y1 + (y2 - y1) * (et - x1)/(x2 - x1));      
  
  y1 = sigmaHD[ie][ieta]; 
  y2 = sigmaHD[ie+1][ieta]; 
  
  if(debug) {
    //  cout.width(6);
  LogDebug("FastCalorimetry") << endl
       << " HCALResponse::interHD sigma" << endl
       << " x, x1-x2, y1-y2 = " 
       << et << ", " << x1 <<"-" << x2 << " " << y1 <<"-" << y2 << endl; 
  
  }
 
  sigma = et * (y1 + (y2 - y1) * (et - x1)/(x2 - x1));      


  if(debug) {
    //cout.width(6);
  LogDebug("FastCalorimetry") << endl
       << " HCALResponse::interHD " << endl
       << " et, ie, ieta = " << et << " " << ie << " " << ieta << endl
       << " mean, sigma  = " << mean << " " << sigma << endl; 
  }

}

void HCALResponse::interEM(double e, int ie, int ieta)
{ 
  double y1 = meanEM[ie][ieta]; 
  double y2 = meanEM[ie+1][ieta]; 
  double x1 = eGridEM[ie];
  double x2 = eGridEM[ie+1];
  
  if(debug) {
    //  cout.width(6);
  LogDebug("FastCalorimetry") << endl
       << " HCALResponse::interEM mean " << endl
       << " x, x1-x2, y1-y2 = " 
       << e << ", " << x1 <<"-" << x2 << " " << y1 <<"-" << y2 << endl; 
  
  }

  mean =  e * (y1 + (y2 - y1) * (e - x1)/(x2 - x1));      
  
  y1 = sigmaEM[ie][ieta]; 
  y2 = sigmaEM[ie+1][ieta]; 
  
  if(debug) {
    //  cout.width(6);
  LogDebug("FastCalorimetry") << endl
       << " HCALResponse::interEM sigma" << endl
       << " x, x1-x2, y1-y2 = " 
       << e << ", " << x1 <<"-" << x2 << " " << y1 <<"-" << y2 << endl; 
  
  }

  sigma = e * (y1 + (y2 - y1) * (e - x1)/(x2 - x1));      
}



// Old parametrization for hadrons
double HCALResponse::getHCALEnergyResolution(double e, int hit){
  
   if(hit==hcforward) 
     return e *sqrt( RespPar[VFCAL][1][0]*RespPar[VFCAL][1][0] / e + 
		     RespPar[VFCAL][1][1]*RespPar[VFCAL][1][1] );
   else
     return  e * sqrt( RespPar[HCAL][hit][0]*RespPar[HCAL][hit][0]/(e)
                     + RespPar[HCAL][hit][1]*RespPar[HCAL][hit][1]);   

}

// Old parameterization of the calo response to hadrons
double HCALResponse::getHCALEnergyResponse(double e, int hit){

  double s = eResponseScale[hit];
  double n = eResponseExponent;
  double p = eResponsePlateau[hit];
  double c = eResponseCoefficient;

  double response = e * p / (1+c*exp(n * log(s/e)));

  if(response<0.) response = 0.;

  return response;
}

// old parameterization of the HF response to electrons
double HCALResponse::getHFEnergyResolution(double EGen)
{
    return EGen *sqrt( RespPar[VFCAL][0][0]*RespPar[VFCAL][0][0] / EGen + 
		       RespPar[VFCAL][0][1]*RespPar[VFCAL][0][1] );
}  


#ifdef IJKLMMNOP // This stuff was moved to Calorimetry.cfi
// default values for old prameterizations
const double HCALResponse::RespByDefault[3][2][3] = {
//Numbers taken from Calorimetry TDR's.
  //ECAL - dummy, the numbers are in  FASTEnergyReconstructor.cc
  //Stochastic Constant Noise 
 {{3*0.},   //barrel
  {3*0.}}, //endcap
  //HCAL
 {{1.22,       0.05,       0},    //barrel
  {1.30,       0.05,       0}},   //endcap (R. Harris - 17-oct-2004)
  //VFCAL
 {{1.82,       0.09,    0},  //hadrons
  {1.38,       0.05,    0}}  //electrons & photons
};
#endif
