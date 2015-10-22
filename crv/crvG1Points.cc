/*
 * Copyright 2015 Scientific Computation Research Center
 *
 * This work is open source software, licensed under the terms of the
 * BSD license as described in the LICENSE file in the top-level directory.
 */

#include "crvBezier.h"
#include "crvTables.h"
#include <cassert>

namespace crv {

static void getGregoryTriangleTransform(apf::NewArray<double> & c)
{
  double f4[45] = {
      1.473866405971784,-0.4873245458152482,-0.4873245458152483,-2.157170326583087,
      -0.7895371825786641,1.002268609355065,0.4569922360188257,0.4160673296503333,
      0.4569922360188257,1.002268609355065,-0.7895371825786643,-2.157170326583087,
      7.066481928037422,-2.00343662222666,-2.00343662222666,
      -0.4873245458152483,1.473866405971784,-0.4873245458152482,1.002268609355065,
      -0.7895371825786646,-2.157170326583087,-2.157170326583087,-0.7895371825786643,
      1.002268609355065,0.4569922360188255,0.4160673296503333,0.4569922360188255,
      -2.00343662222666,7.066481928037421,-2.00343662222666,
      -0.4873245458152483,-0.4873245458152481,1.473866405971784,0.4569922360188258,
      0.4160673296503332,0.4569922360188255,1.002268609355065,-0.7895371825786646,
      -2.157170326583087,-2.157170326583088,-0.7895371825786643,1.002268609355065,
      -2.00343662222666,-2.00343662222666,7.066481928037422};

  int nbBezier = 3;
  int niBezier = 15;

  int nb = 6;
  int ni = 18;
  c.allocate(ni*nb);

  int map[3] = {1,2,0};
  // copy the bezier point locations
  for(int i = 0; i < nbBezier; ++i){
    for(int j = 0; j < niBezier; ++j)
      c[i*ni+j] = f4[i*niBezier+j];
    for(int j = niBezier; j < ni; ++j)
      c[i*ni+j] = 0.;
  }

  for(int i = nbBezier; i < nb; ++i){
    for(int j = 0; j < niBezier; ++j)
      c[i*ni+j] = f4[map[i-nbBezier]*niBezier+j];
    for(int j = niBezier; j < ni; ++j)
      c[i*ni+j] = 0.;
  }

}

static void getGregoryTetTransform(apf::NewArray<double> & c)
{
  assert(getBlendingOrder(apf::Mesh::TET) == 0);
  double t4[47] = {
      -0.665492638178598,-0.665492638178598,-0.665492638178598,-0.665492638178598,
      0.697909481209196,0.496340368840329,0.697909481209197,0.697909481209196,
      0.496340368840329,0.697909481209196,0.697909481209196,0.49634036884033,
      0.697909481209196,0.697909481209196,0.496340368840329,0.697909481209196,
      0.697909481209196,0.496340368840329,0.697909481209196,0.697909481209196,
      0.496340368840329,0.697909481209196,
      -0.764902170896025,-0.764902170896025,-0.764902170896025,-0.764902170896025,
      -0.764902170896025,-0.764902170896025,
      -0.764902170896025,-0.764902170896025,-0.764902170896025,-0.764902170896025,
      -0.764902170896025,-0.764902170896025,
      -0.764902170896025,-0.764902170896025,-0.764902170896025,-0.764902170896025,
      -0.764902170896025,-0.764902170896025,
      -0.764902170896025,-0.764902170896025,-0.764902170896025,-0.764902170896025,
      -0.764902170896025,-0.764902170896025,
      10.6666666666667,
  };

  c.allocate(47);
  for (int j = 0; j < 47; ++j)
    c[j] = t4[j];
}

static void getBlendedGregoryTriangleTransform(int blend,
    apf::NewArray<double> & c)
{
  double f4_1[36] = {
      -0.9652522174935585,-0.4749278524285263,-0.474927852428525,0.9544734242852975,
      0.2114582311122364,-0.3558323000932463,0.3162670836759962,0.6623750443900446,
      0.3162670836759965,-0.3558323000932466,0.2114582311122365,0.9544734242852969,
      -0.4749278524285266,-0.9652522174935582,-0.4749278524285267,-0.3558323000932466,
      0.2114582311122362,0.9544734242852964,0.9544734242852971,0.2114582311122362,
      -0.3558323000932463,0.3162670836759968,0.6623750443900446,0.3162670836759962,
      -0.4749278524285246,-0.4749278524285249,-0.9652522174935577,0.316267083675996,
      0.6623750443900441,0.3162670836759964,-0.3558323000932463,0.2114582311122363,
      0.9544734242852962,0.9544734242852965,0.211458231112236,-0.3558323000932463};

  double f4_2[36] = {
      -0.4869153443383633,0.0003941257550036311,0.0003941257550037788,0.593245256335027,
      0.1810102640100068,-0.1679256083483691,0.04693453767005338,0.1795981934949205,
      0.04693453767005338,-0.167925608348369,0.181010264010007,0.5932452563350273,
      0.0003941257550036313,-0.4869153443383633,0.0003941257550035421,-0.1679256083483691,
      0.1810102640100067,0.5932452563350268,0.5932452563350268,0.1810102640100067,
      -0.1679256083483691,0.04693453767005345,0.1795981934949204,0.04693453767005323,
      0.0003941257550036312,0.0003941257550038111,-0.486915344338363,0.04693453767005334,
      0.1795981934949204,0.04693453767005355,-0.1679256083483689,0.1810102640100068,
      0.593245256335027,0.5932452563350267,0.1810102640100066,-0.1679256083483692};
  double* f4[2] = {f4_1,f4_2};

  int nbBezier = 3;
  int niBezier = 12;

  int nb = 6;
  int ni = 12;
  c.allocate(ni*nb);

  int map[3] = {1,2,0};
  // copy the bezier point locations
  for(int i = 0; i < nbBezier; ++i){
    for(int j = 0; j < niBezier; ++j)
      c[i*ni+j] = f4[blend-1][i*niBezier+j];
  }
  for(int i = nbBezier; i < nb; ++i){
    for(int j = 0; j < niBezier; ++j)
      c[i*ni+j] = f4[blend-1][map[i-nbBezier]*niBezier+j];
  }
}

static void getBlendedGregoryTetTransform(int blend,
    apf::NewArray<double> & c)
{
  assert(getBlendingOrder(apf::Mesh::TET) == 0);
  double t4_1[46] = {
      1.921296296296296,1.921296296296296,1.921296296296296,1.921296296296296,
      -0.7098765432098767,-1.064814814814814,-0.7098765432098767,-0.7098765432098767,
      -1.064814814814814,-0.7098765432098767,-0.7098765432098767,-1.064814814814814,
      -0.7098765432098767,-0.7098765432098767,-1.064814814814814,-0.7098765432098767,
      -0.7098765432098767,-1.064814814814814,-0.7098765432098767,-0.7098765432098767,
      -1.064814814814814,-0.7098765432098767,
      0.342592592592593,0.342592592592593,0.342592592592593,
      0.342592592592593,0.342592592592593,0.342592592592593,
      0.342592592592593,0.342592592592593,0.342592592592593,
      0.342592592592593,0.342592592592593,0.342592592592593,
      0.342592592592593,0.342592592592593,0.342592592592593,
      0.342592592592593,0.342592592592593,0.342592592592593,
      0.342592592592593,0.342592592592593,0.342592592592593,
      0.342592592592593,0.342592592592593,0.342592592592593};
  double t4_2[46] = {
      0.3472222222222222,0.3472222222222222,0.3472222222222222,0.3472222222222222,
      -0.2407407407407403,-0.3611111111111111,-0.2407407407407404,-0.2407407407407404,
      -0.3611111111111111,-0.2407407407407404,-0.2407407407407404,-0.3611111111111111,
      -0.2407407407407404,-0.2407407407407404,-0.3611111111111111,-0.2407407407407404,
      -0.2407407407407404,-0.3611111111111111,-0.2407407407407404,-0.2407407407407404,
      -0.3611111111111111,-0.2407407407407404,
      0.388888888888888,0.3888888888888888,0.3888888888888888,0.3888888888888888,
      0.3888888888888888,0.3888888888888888,
      0.3888888888888888,0.3888888888888888,0.3888888888888888,0.3888888888888888,
      0.3888888888888888,0.3888888888888888,
      0.388888888888888,0.3888888888888888,0.3888888888888888,0.3888888888888888,
      0.3888888888888888,0.3888888888888888,
      0.3888888888888888,0.3888888888888888,0.3888888888888888,0.3888888888888888,
      0.3888888888888888,0.3888888888888888};
  double* t4[2] = {t4_1,t4_2};
  c.allocate(46);
  for (int j = 0; j < 46; ++j)
    c[j] = t4[blend-1][j];
}

void getGregoryTransformationCoefficients(int type,
    apf::NewArray<double>& c){
  if(type == apf::Mesh::EDGE){
    c.allocate(15);
    double e4[15] = {
        -1.52680420766155,-0.25,3.37567603341243,-1.28732567375034,
        0.688453847999458,1.01786947177436,1.01786947177436,-2.70941992094126,4.38310089833379,
        -2.70941992094126,-0.25,-1.52680420766155,0.688453847999459,-1.28732567375034,
        3.37567603341243};
    for(int i = 0; i < 15; ++i)
      c[i] = e4[i];
  }
  else if(type == apf::Mesh::TRIANGLE)
    getGregoryTriangleTransform(c);
  else if(type == apf::Mesh::TET)
    getGregoryTetTransform(c);
}

void getGregoryBlendedTransformationCoefficients(int blend, int type,
    apf::NewArray<double>& c){

  if(type == apf::Mesh::TRIANGLE)
    getBlendedGregoryTriangleTransform(blend,c);
  else if(type == apf::Mesh::TET)
    getBlendedGregoryTetTransform(blend,c);
}

}
