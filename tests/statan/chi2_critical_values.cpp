/*
  GNU Gama C++ library tests/statan
  Copyright (C) 2018  Ales Cepek <cepek@gnu.org>

  This file is part of the GNU Gama C++ library
  
  GNU Gama is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  GNU Gama is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with GNU Gama.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Math/Business/statan.h>

#include <iostream>
#include <vector>
#include <cmath>

using namespace GNU_gama;
using namespace std;

/*
  Testing data generated from the Octave script:

  % chi square - critical values

  % degrees of freedom
  dof = [1 2 3 4 5 10 20 30 50 100];

  N = 100;
  x = N-1:-1:80;
  e = ones(1,columns(x));

  printf('  std::vector<double> X = {\n');
  printf('    %6.3f,\n', x/N);
  printf('  };\n\n');

  for i = dof
  c = chi2inv(e - x/N, i);
  
  printf('{\n');
  printf('  std::vector<double> Y = {\n');
  printf('    %20.16f,\n', c);
  printf('  };\n\n');
  printf('  int dof = %d;\n', i);
  printf('  max_avg(dof, X, Y);\n');
  printf('}\n\n');
  endfor

*/

/* 
   R language

   x <- c(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20)/100

   > qchisq(x,1)
    [1] 0.0001570879 0.0006284502 0.0014143833 0.0025153819 0.0039321400
    [6] 0.0056655521 0.0077167155 0.0100869322 0.0127777117 0.0157907741
   [11] 0.0191280539 0.0227917040 0.0267841001 0.0311078460 0.0357657792
   [16] 0.0407609769 0.0460967633 0.0517767164 0.0578046765 0.0641847547
   > qchisq(x,2)
    [1] 0.02010067 0.04040541 0.06091841 0.08164399 0.10258659 0.12375081
    [7] 0.14514139 0.16676322 0.18862136 0.21072103 0.23306763 0.25566674
   [13] 0.27852413 0.30164578 0.32503786 0.34870677 0.37265916 0.39690188
   [19] 0.42144206 0.44628710
   > qchisq(x,3)
    [1] 0.1148318 0.1848318 0.2450987 0.3001514 0.3518463 0.4011734 0.4487387
    [8] 0.4949476 0.5400880 0.5843744 0.6279721 0.6710124 0.7136022 0.7558302
   [15] 0.7977714 0.8394903 0.8810430 0.9224790 0.9638427 1.0051740
   > qchisq(x,4)
    [1] 0.2971095 0.4293982 0.5350537 0.6271452 0.7107230 0.7883730 0.8616294
    [8] 0.9314916 0.9986541 1.0636232 1.1267820 1.1884291 1.2488030 1.3080986
   [15] 1.3664772 1.4240753 1.4810094 1.5373805 1.5932768 1.6487766
   > qchisq(x,5)
    [1] 0.5542981 0.7518889 0.9030560 1.0313230 1.1454762 1.2499152 1.3472068
    [8] 1.4390003 1.5264282 1.6103080 1.6912521 1.7697339 1.8461285 1.9207393
   [15] 1.9938163 2.0655688 2.1361740 2.2057845 2.2745325 2.3425343
   > qchisq(x,10)
    [1] 2.558212 3.059051 3.412069 3.696541 3.940299 4.156724 4.353392 4.535050
    [9] 4.704892 4.865182 5.017588 5.163382 5.303554 5.438899 5.570059 5.697569
   [17] 5.821873 5.943351 6.062326 6.179079
   > qchisq(x,20)
    [1]  8.260398  9.236699  9.897079 10.415364 10.850811 11.231378 11.572660
    [8] 11.884321 12.172797 12.442609 12.697066 12.938668 13.169356 13.390672
   [15] 13.603860 13.809943 14.009775 14.204073 14.393452 14.578439
   > qchisq(x,30)
    [1] 14.95346 16.30617 17.20762 17.90828 18.49266 19.00038 19.45343 19.86537
    [9] 20.24520 20.59923 20.93207 21.24718 21.54724 21.83439 22.11034 22.37651
   [17] 22.63405 22.88396 23.12707 23.36411
   > qchisq(x,50)
    [1] 29.70668 31.66386 32.95091 33.94259 34.76425 35.47430 36.10500 36.67620
    [9] 37.20103 37.68865 38.14573 38.57731 38.98725 39.37864 39.75393 40.11515
   [17] 40.46398 40.80183 41.12990 41.44921
   > qchisq(x,100)
    [1] 70.06489 73.14218 75.14185 76.67050 77.92947 79.01207 79.96967 80.83376
    [9] 81.62509 82.35814 83.04342 83.68882 84.30044 84.88308 85.44061 85.97617
   [17] 86.49239 86.99145 87.47524 87.94534
 */

double MAX = 0;

void max_avg(int dof, vector<double>& X, vector<double>& Y)
{
  double max = 0, sum = 0, diff {};
  for (std::size_t i=0; i<X.size(); i++) {
    diff = Chi_square(X[i], dof) - Y[i];
    sum += diff;
    if (abs(diff) > abs(max)) max = diff;
  }
  sum /= X.size();
  cout << "Chi square distribution - critical values" << endl;
  cout << "degrees of freedom = " << dof << endl;
  cout << "diff max = " << max << "  avg = " << sum << "\n\n";

  if (abs(max) > abs(MAX)) MAX = max;
}

int main()
{
  std::vector<double> X = {
     0.990,
     0.980,
     0.970,
     0.960,
     0.950,
     0.940,
     0.930,
     0.920,
     0.910,
     0.900,
     0.890,
     0.880,
     0.870,
     0.860,
     0.850,
     0.840,
     0.830,
     0.820,
     0.810,
     0.800
  };

{
  std::vector<double> Y = {
      0.0001570878579097,
      0.0006284501612837,
      0.0014143833008118,
      0.0025153819343540,
      0.0039321400000195,
      0.0056655521405274,
      0.0077167155449520,
      0.0100869322157760,
      0.0127777116710530,
      0.0157907740934312,
      0.0191280539398314,
      0.0227917040277124,
      0.0267841001161233,
      0.0311078460021464,
      0.0357657791558977,
      0.0407609769200010,
      0.0460967633024077,
      0.0517767163946213,
      0.0578046764508420,
      0.0641847546673015
  };

  int dof = 1;
  max_avg(dof, X, Y);
}

{
  std::vector<double> Y = {
      0.0201006717070029,
      0.0404054146350389,
      0.0609184149694171,
      0.0816439890405103,
      0.1025865887751012,
      0.1237508074361750,
      0.1451413856696707,
      0.1667632178781020,
      0.1886213589424825,
      0.2107210313156526,
      0.2330676325119030,
      0.2556667430197698,
      0.2785241346670154,
      0.3016457794691673,
      0.3250378589955498,
      0.3487067742895556,
      0.3726591563829870,
      0.3969018774476765,
      0.4214420626313051,
      0.4462871026284194
  };

  int dof = 2;
  max_avg(dof, X, Y);
}

{
  std::vector<double> Y = {
      0.1148318018991172,
      0.1848318190286583,
      0.2450987087102979,
      0.3001514187972258,
      0.3518463177492716,
      0.4011733580006246,
      0.4487387013269667,
      0.4949475616972147,
      0.5400879929255636,
      0.5843743741551832,
      0.6279720751256036,
      0.6710124156622944,
      0.7136022277875729,
      0.7558302325418172,
      0.7977714442447216,
      0.8394903035337418,
      0.8810429632085067,
      0.9224789930777952,
      0.9638426764061239,
      1.0051740130523492
  };

  int dof = 3;
  max_avg(dof, X, Y);
}

{
  std::vector<double> Y = {
      0.2971094805065320,
      0.4293981900158692,
      0.5350536732353304,
      0.6271451607053676,
      0.7107230213973244,
      0.7883729629976495,
      0.8616293678335851,
      0.9314916023280778,
      0.9986541214994746,
      1.0636232167792239,
      1.1267819797944987,
      1.1884290533965098,
      1.2488030394442671,
      1.3080985616689185,
      1.3664772261418809,
      1.4240753309926935,
      1.4810094321099800,
      1.5373804523431582,
      1.5932767756705308,
      1.6487766180659691
  };

  int dof = 4;
  max_avg(dof, X, Y);
}

{
  std::vector<double> Y = {
      0.5542980767282775,
      0.7518889347790670,
      0.9030559890121299,
      1.0313229709926619,
      1.1454762260617699,
      1.2499151560848567,
      1.3472067792290034,
      1.4390002559287454,
      1.5264282405316998,
      1.6103079869623229,
      1.6912520813315650,
      1.7697338956026878,
      1.8461284847420796,
      1.9207393085121283,
      1.9938163464530487,
      2.0655687640022569,
      2.1361740054338756,
      2.2057844721349924,
      2.2745325261867495,
      2.3425343058411210
  };

  int dof = 5;
  max_avg(dof, X, Y);
}

{
  std::vector<double> Y = {
      2.5582121601872063,
      3.0590514108687183,
      3.4120685471200218,
      3.6965414449566891,
      3.9402991361190609,
      4.1567242918149372,
      4.3533917316620592,
      4.5350496664300808,
      4.7048920849959170,
      4.8651820519253288,
      5.0175884255504046,
      5.1633816355160551,
      5.3035542442640047,
      5.4388987027239724,
      5.5700594442159623,
      5.6975689178653317,
      5.8218731997854993,
      5.9433506284435262,
      6.0623256444864673,
      6.1790792560393930
  };

  int dof = 10;
  max_avg(dof, X, Y);
}

{
  std::vector<double> Y = {
      8.2603983325464050,
      9.2366985551262051,
      9.8970792932456568,
     10.4153641665232719,
     10.8508113941825979,
     11.2313776610105265,
     11.5726598473937354,
     11.8843209681890460,
     12.1727970769669707,
     12.4426092104500796,
     12.6970657563547071,
     12.9386676415893920,
     13.1693561272567941,
     13.3906716328224231,
     13.6038595449049549,
     13.8099430279907356,
     14.0097745287331428,
     14.2040730851385053,
     14.3934519188780925,
     14.5784392170705406
  };

  int dof = 20;
  max_avg(dof, X, Y);
}

{
  std::vector<double> Y = {
     14.9534565284554475,
     16.3061747167292665,
     17.2076246819549148,
     17.9082785497551171,
     18.4926609819534775,
     19.0003826429044977,
     19.4534299572824985,
     19.8653683497776150,
     20.2452014603336607,
     20.5992346145853418,
     20.9320693042909767,
     21.2471753118571307,
     21.5472397414853241,
     21.8343902392494904,
     22.1103434054488339,
     22.3765067350217883,
     22.6340506049425443,
     22.8839603385100716,
     23.1270746525299948,
     23.3641145737900970
  };

  int dof = 30;
  max_avg(dof, X, Y);
}

{
  std::vector<double> Y = {
     29.7066826988412700,
     31.6638623749192263,
     32.9509116313112571,
     33.9425899971795317,
     34.7642516835016551,
     35.4742985369530714,
     36.1050029508177062,
     36.6762043988739279,
     37.2010301101400032,
     37.6886483939784895,
     38.1457339437840517,
     38.5773087896031157,
     38.9872542297507110,
     39.3786375996076785,
     39.7539291478940626,
     40.1151507619876142,
     40.4639808359236781,
     40.8018300090432646,
     41.1298970219194118,
     41.4492106736201364
  };

  int dof = 50;
  max_avg(dof, X, Y);
}

{
  std::vector<double> Y = {
     70.0648949253997415,
     73.1421829351888562,
     75.1418530935510205,
     76.6705008897637867,
     77.9294651650171915,
     79.0120669385481023,
     79.9696717766679370,
     80.8337550678761261,
     81.6250861257991573,
     82.3581358123570197,
     83.0434165263241368,
     83.6888222801041337,
     84.3004429241429136,
     84.8830829798980488,
     85.4406054505581523,
     85.9761672537264019,
     86.4923850061186954,
     86.9914546085052081,
     87.4752393338597187,
     87.9453359227509281
  };

  int dof = 100;
  max_avg(dof, X, Y);
}

 // results from Chi_square() function are only approximate 
 cout << "maximal diff = " << MAX << endl;
 return abs(MAX) < 1e-3 ? 0 : 1;
}
