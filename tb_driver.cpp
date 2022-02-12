#include "tb_driver.h"
#include <iomanip>
#include <fstream>
void tb_driver::generate_reset(void){
     //генерация ресета
    rst.write(1);
    wait(clk->posedge_event());
    rst.write(0);
    wait(clk->posedge_event());
};

void tb_driver::generate_biases(void) {
     double C[BIASES]={0.0691566914, 0.0087906541, 0.0805735663, 0.0110252723, 0.0001045055, 0.0136789260, 0.0097796526, -0.0016237695, 0.0948802680, 0.0211041402, -0.0034900929, 0.0307639018, 0.0448412858, -0.0014694935, 
    0.0388998576, 0.0056192460, 0.0029507081, -0.0039654267, 0.0023400388, 0.0503108688, -0.0020105073, -0.0073869051, 0.0047671571, 0.0638136268, 0.0234106053, 0.0156689044, -0.0022752020, 0.0103466120, 0.0103880670, 
    0.0530131310, -0.0050606704, 0.0142416079};

    double biases_flattened[BIASES];    
     for (int i=0; i<BIASES; i++){
         biases_flattened[i]=C[i];
     } 

    biases.write(0);
    biases_vld.write(0);
    
    double biases_tmp;
    for (int i=0;i<BIASES;i++){
        biases_vld.write(1);
        biases_tmp=biases_flattened[i];
        biases.write(biases_tmp);
        do{
            wait(clk->posedge_event());
        }while(!biases_rdy.read());
        biases_vld.write(0);
    }
    biases.write(0);
};

void tb_driver::generate_kernel(void) {
        
    //это входные матрицы кернела, баесов и изображения
    //нормализация кернела- деление каждого элемента кернела на сумму его элементов
    
    double A[L1][M1][N1] = {-0.1334408224, -0.1392686367, 0.1300974339, -0.0864856467, -0.1083627939, 0.2088107765, -0.1136551201, 0.2401071936, 0.1188950911, -0.0087087248, 0.1201948971, -0.0464182496, 0.0884167776,
     0.2751140296, -0.1375161707, -0.0019576887, 0.2042565495, -0.0708352476, -0.0744715557, -0.1670113057, 0.2235770524, -0.0300364848, -0.0904558152, 0.0767594874, -0.0708144158, -0.1405453682, 0.1387028098, 0.1103126407, 
     0.0334975943, 0.0571969189, 0.1984982193, 0.0466368943, -0.1475086510, 0.1699092090, -0.1381194741, -0.1358170807, 0.1013357565, 0.2410885096, 0.1847932339, -0.0505895279, 0.2524847388, 0.2070971727, -0.0867050141, 
     -0.0405460075, 0.0005758021, 0.1156024709, -0.1317615211, -0.3176175654, 0.1692831367, -0.1006272137, 0.0296136346, 0.2585493326, 0.1361885369, 0.0743186399, 0.2459016889, -0.0832119286, -0.0458432846, 0.1869760603, 
     0.0381098986, 0.1692061424, -0.1310384572, -0.1887856871, 0.1525160372, 0.1823249459, 0.1086513773, -0.0107199261, 0.2136799395, 0.0850499570, 0.0033886177, 0.0657599941, 0.0442086495, -0.0413832068, 0.0949456692, 
     0.1315574348, -0.0945426300, 0.1563679278, 0.0484856404, -0.2048237324, 0.1818284690, -0.0574250929, -0.2427499592, 0.1053847373, 0.2232054025, 0.1813056767, 0.0563280731, -0.0684833825, 0.1469190270, 0.0501676761, 
     -0.2391771823, -0.1222589761, -0.0560020991, -0.0371293351, 0.1111211404, -0.1734506637, -0.0808828026, -0.0866658762, 0.1352173388, 0.2915632129, 0.2167771310, -0.0506454147, 0.1672631055, 0.0986972153, 0.1702887118, 
     0.0083666919, -0.0888174623, -0.0462655686, -0.0724466890, -0.1645785570, 0.0832245573, 0.0272265840, 0.2304167897, -0.2340481728, 0.0753766596, 0.2598103881, -0.2438277900, 0.1313868612, 0.1734100282, 0.0698637590, 
     0.2284591496, 0.2316483557, 0.0095564276, 0.1253672242, 0.2260870188, -0.1349650919, -0.1772097796, 0.0284365471, -0.1485281289, -0.1870962232, -0.1053984836, -0.0633049905, -0.0310533326, 0.1089378595, 0.2280425727, 
     0.1818936765, 0.0312003307, 0.0766758919, 0.1323637217, 0.2507510781, 0.0425814502, 0.0881438479, 0.0689459145, -0.0638374463, -0.1648586988, 0.0282942709, 0.2105543017, 0.1573654711, 0.0828195512, -0.1470266283, 
     0.0382105596, 0.2336969078, -0.3344029188, 0.0427887514, 0.0233102776, 0.0994880497, -0.1221243814, -0.0198040549, -0.0638984591, -0.0659709945, 0.1502571255, 0.1380349249, 0.2589043975, 0.2675340772, 0.1755441576, -0.1680716723, -0.3174853325, 0.1256543994, 0.1293583810, -0.1687576026, 0.1345756799, 0.1560312808, 0.1333180368, -0.0025231766, -0.0219023935, 0.0530890822, -0.2009634376, -0.0140871778, 0.1031824276, -0.0862676129, 0.1540612131, 0.3178539276, -0.1734967530, 0.1997896284, 0.1092780009, -0.0125859212, 0.0868493393, -0.0249132011, 0.0740810707, 0.2514920533, -0.0447467491, 0.0727402717, 0.1659678072, -0.0136705935, 0.0627527013, 0.2041825205, -0.1596026570, 0.1040261686, 0.1767254919, 0.0765110552, 0.1283628941, -0.0458473898, 0.0859541595, 0.1806637794, -0.0669835508, 0.1020670831, 0.1143994555, 0.0653855056, -0.1525852978, 0.2471047789, 0.0717002079, -0.0466487482, 0.2293261439, 0.1141782477, -0.1817942113, 0.2250242978, -0.0656551346, -0.1764259785, -0.1501168609, -0.1625258327, -0.2115994096, 0.1556869894, 0.0055147563, -0.0568647832, 0.0832367986, 0.1775472164, 0.1710466146, 0.0968883559, 0.1371255666, 0.1998959333, -0.2128604203, 0.1471459866, 0.2381323427, -0.1197310537, 0.1620117128, -0.0698607564, 0.0136624426, 0.0277941972, -0.0355514921, -0.0089199571, 0.1341662258, 0.1547280103, 0.0474462211, 0.2654829919, -0.1001522541, -0.2835018039, -0.1740090400, -0.0056355684, -0.0540362336, 0.0455632322, 0.0781389922, 0.2156127393, 0.1958227754, 0.0878010616, 0.1345463097, -0.0082714921, -0.0327090099, 0.1379627734, 0.1397219300, 0.1212623715, -0.1272477806, 0.0742204934, 0.1599699408, -0.3537354171, -0.1457158923, 0.0012764280, -0.1180670559, 0.0188694298, 0.2253460735, -0.0256480854, 0.1669477671, 0.1762448251, 0.0061263377, -0.0556254387, 0.1376881301, 0.1117793992, -0.0621911995, 0.2501133084, -0.1320263296, 0.1021381915, 0.1587602794, -0.0839346871, 0.0418825187, -0.1973469704, 0.1088332608, 0.0197435841, 0.0327958204, -0.1766695082, 0.2609765530, 0.2837161720};

    cout << endl;
    double kernel_flattened[KER];

     for (int j = 0; j < L1; ++j) {
         for (int i = 0; i < M1; ++i) {
            for (int k = 0; k < N1; ++k) {
                    kernel_flattened[j * M1 * N1 + i * M1 + k]=A[j][i][k];
            }
         }
     }
  
    kernel.write(0);
    kernel_vld.write(0);

    //поэлементная передача данных на порты
     double kernel_tmp;
     for (int i=0; i<KER; i++){
         kernel_vld.write(1);
         kernel_tmp=kernel_flattened[i];
         kernel.write(kernel_tmp);
         do{
             wait(clk->posedge_event());
        }while(!kernel_rdy.read());
        kernel_vld.write(0);
     }
     kernel.write(0);
};


void tb_driver::generate_image(void){

    double B[M2][N2] = { 0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.3921568692,0.5019608140,0.5019608140,0.5019608140,0.3921568692,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.1294117719,0.3568627536,0.3568627536,0.3568627536,0.5803921819,0.6431372762,0.6431372762,0.7686274648,0.7843137383,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.1372549087,0.2156862766,0.4196078479,0.7843137383,0.7843137383,0.7843137383,0.1686274558,0.0000000000,0.0000000000,0.2823529541,0.6156862974,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0549019612,0.6666666865,0.9647058845,0.5960784554,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.7294117808,0.9764705896,0.5372549295,0.0470588244,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0549019612,0.7294117808,0.9529411793,0.4941176474,0.1058823541,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0705882385,0.9294117689,0.4941176474,0.1294117719,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0705882385,0.9294117689,0.2156862766,0.0000000000,0.2509804070,0.5019608140,0.5019608140,0.5019608140,0.5019608140,0.5019608140,0.1803921610,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0705882385,0.9294117689,0.2156862766,0.2313725501,0.6784313917,0.8705882430,0.6431372762,0.6431372762,0.6431372762,0.9215686321,0.5882353187,0.3568627536,0.3568627536,0.2784313858,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0705882385,0.9294117689,0.3843137324,0.5803921819,0.7843137383,0.5058823824,0.0000000000,0.0000000000,0.0000000000,0.6156862974,0.7843137383,0.7843137383,0.7843137383,0.6627451181,0.2156862766,0.2156862766,0.0470588244,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0705882385,0.9294117689,0.9450980425,0.3333333433,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.2000000030,0.9333333373,0.9960784316,0.2156862766,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0705882385,0.9294117689,0.2705882490,0.0235294122,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0156862754,0.1372549087,0.9333333373,0.2156862766,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0705882385,0.9294117689,0.8313725591,0.2823529541,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0705882385,0.9294117689,0.2156862766,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0235294122,0.3333333433,0.8627451062,0.7686274648,0.3215686381,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0705882385,0.9294117689,0.2156862766,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.3921568692,0.8235294223,0.5019608140,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.1098039225,0.5372549295,0.9647058845,0.2156862766,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.6431372762,0.6784313917,0.2313725501,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.1294117719,0.4941176474,0.9764705896,0.6666666865,0.1372549087,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.5058823824,0.8941176534,0.7176470757,0.1686274558,0.0000000000,0.0000000000,0.0000000000,0.1372549087,0.2156862766,0.4941176474,0.9529411793,0.7294117808,0.0549019612,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.4666666687,0.9529411793,0.8000000119,0.0705882385,0.0705882385,0.0705882385,0.6666666865,0.9647058845,0.9294117689,0.7294117808,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0352941193,0.4039215744,0.9843137264,0.9294117689,0.9294117689,0.9294117689,0.9764705896,0.5372549295,0.0705882385,0.0549019612,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0745098069,0.2156862766,0.2156862766,0.2156862766,0.2156862766,0.2156862766,0.1058823541,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
    0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,0.0000000000,
        };

    double image_flattened[IMG]={0};
    for (int i = 0; i < M2; ++i) {
            for (int k = 0; k < N2; ++k) {
                image_flattened[ i * M2 + k]=B[i][k];
               // cout<<B[i][k]<<" ";
            }
            //cout<<endl;
    }
    /*  cout<<"------------image_flattened-----------"<<endl;
    for (int i=0;i<IMG;i++){
        cout<< image_flattened[IMG]<< " ";
    } 
     */
    //поэлементная передача данных на порты
    image.write(0);
    image_vld.write(0);
     double image_tmp;
     for (int i=0; i<IMG; i++){
         image_vld.write(1);
         image_tmp=image_flattened[i];
         
         image.write(image_tmp);
         do{
             wait(clk->posedge_event());
        }while(!image_rdy.read());
        image_vld.write(0);
     }
    image.write(0);

};

void tb_driver::conv_2d_1_sink(void){

    //создаём файл для вывода
    /*char conv_2d_1_output_file[21632];
    sprintf(conv_2d_1_output_file, " ./conv_2d_1_output_recieved.dat");
    outfp=fopen(conv_2d_1_output_file, "w");
    if (outfp == NULL){
        cout<<" Невозможно открыть conv_2d_1_output_recieved.dat для записи"<<endl;
        exit(0);
    }*/



    //инициализируем хэндшейк
    conv_2d_1_result_rdy.write(0);
    wait(clk->posedge_event());

    double conv_2d_1_result_flattened[CONV_ED];
    
    /* Поскольку в SystemC есть ограничение по выделяемой памти создаём динамический 
    массив через тройной указатель для хранения результатов слоя*/
    //double conv_2d_1_result_arr[L3][M3][N3];
    double*** conv_2d_1_result_arr = new double**[L3];
    for (int i=0; i<L3;i++){
        conv_2d_1_result_arr[i] = new double*[M3];
        for (int j=0;j<M3;j++){
            conv_2d_1_result_arr[i][j] = new double[N3];
        }
    }

    while(true){
        for(int i=0;i<CONV_ED+1;i++){
            conv_2d_1_result_rdy.write(1);
            do{
                wait(clk->posedge_event());
            }while (!conv_2d_1_result_vld.read());
            conv_2d_1_result_flattened[i]=conv_2d_1_result.read();
            conv_2d_1_result_rdy.write(0);
            //fprintf(outfp, "%f\n", conv_2d_1_result.read());
        }
        cout<<"@" << sc_time_stamp() <<" convolution data recieved"<<endl;
		/* 
        for (int i=0;i<CONV_ED;i++){
            cout<<conv_2d_1_result_flattened[i]<<" ";
        } */
        
         for (int i = 0; i < L3; i++) {
				for (int j = 0; j < M3; j++) {
					for (int k = 0; k < N3; k++) {
						conv_2d_1_result_arr[i][j][k]=conv_2d_1_result_flattened[i*N3*M3+j*N3+k+1]; 
					}
				}
		} 
        
       /*  cout<<"[отладочный вывод][DRI_TB] результат:"<<endl;
        for (int i = 0; i < L3; i++) {
            for (int j = 0; j < M3; j++) {
                for (int k = 0; k < N3; k++) {
                    cout <<std::setprecision(1)<< conv_2d_1_result_arr[i][j][k] << " ";
                }
                cout << endl;
            }
            cout << "_________________" << endl;
            cout << endl << endl;
        } */

        for (int i = 0; i < L3; i++) {
            for (int j = 0; j < M3; j++) {               
                delete[] conv_2d_1_result_arr[i][j];
            }
            delete[] conv_2d_1_result_arr[i];
		}
        delete[] conv_2d_1_result_arr; 
        //sc_stop();
    }
};

void tb_driver::generate_kernel2(void){
    
    
//передача кернелов для conv_2d_2 из файла в динамический массив kernel_flattened2
    std::fstream file( kernel2file, std::fstream::in);
    if (!file){
        cout<<"Файл" << kernel2file << "не найден\n";
    }
  
 
    for (int i = 0; i < KER2; i++){
        file >> kernel2_flattened[i];
        
    }

    /*  for (int i = 0; i < KER2; i++){
        cout << kernel2_flattened[i] << ' ';
        
    } */
   
    file.close(); 


    kernel2.write(0);
    kernel2_vld.write(0);

    //поэлементная передача данных на порты
     double kernel2_tmp;
     for (int i = 0; i < KER2; i++){
         kernel2_vld.write(1);
         kernel2_tmp=kernel2_flattened[i];
         kernel2.write(kernel2_tmp);
         do{
             wait(clk->posedge_event());
        }while(!kernel2_rdy.read());
        kernel2_vld.write(0);
     }
     kernel2.write(0);
     
};

void tb_driver::generate_biases2(void){
    double E[BIASES2]={-0.0319721363, -0.0447157994, -0.0168587752, -0.0115468036, -0.0336905383, -0.0329694636, 0.0083970055, -0.0143109867, -0.0381064825, -0.0176094137, -0.0404341891, -0.0157356858, -0.0006183723, 
    0.0078244302, -0.0062869303, -0.0172499456, -0.0435957015, -0.0604985617, -0.0341236703, 0.0326732211, -0.0488306917, -0.0161325540, -0.0398999825, 0.0104548279, -0.0517216362, 0.0122273089, -0.0157566220, -0.0344855376, 
    -0.0323644243, -0.0295408815, -0.0563105606, -0.0530514717, 0.0186393186, -0.0245105252, -0.0459587127, -0.0044532772, 0.0402350910, -0.0090043899, -0.0605863631, 0.0222345572, -0.0693054795, -0.0273299646, -0.0461276248, 
    -0.0529682301, -0.0445274785, 0.0169386603, -0.0419715308, -0.0207674168, -0.0417286605, -0.0607198365, -0.0494330451, -0.0179587528, -0.0164895467, -0.0572269782, -0.0272609033, 0.0037255008, -0.0060905344, -0.0228936467, 
    -0.0478452966, 0.0074354270, 0.0810477808, -0.0312006790, -0.0443888307, -0.0061311815 };

    double biases2_flattened[BIASES2];    
     for (int i = 0; i < BIASES2; i++){
         biases2_flattened[i]=E[i];
     } 

    biases2.write(0);
    biases2_vld.write(0);
    
    double biases2_tmp;
    for (int i = 0 ;i < BIASES2;i++){
        biases2_vld.write(1);
        biases2_tmp=biases2_flattened[i];
        biases2.write(biases2_tmp);
        do{
            wait(clk->posedge_event());
        }while(!biases2_rdy.read());
        biases2_vld.write(0);
    }
    biases2.write(0);
};
