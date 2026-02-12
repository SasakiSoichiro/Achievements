xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 18;
 1.28568;68.29882;8.77950;,
 7.66436;68.29882;0.00000;,
 1.28568;68.29882;-8.77950;,
 -9.03524;68.29882;-5.42603;,
 -9.03524;68.29882;5.42603;,
 -1.56696;57.82297;0.00000;,
 8.56287;10.22157;-33.08855;,
 -30.33504;10.22157;-20.44985;,
 -24.45012;41.42572;-16.62560;,
 7.17363;41.42572;-26.90076;,
 -30.33504;10.22157;20.44985;,
 -24.45012;41.42572;16.62560;,
 8.56287;10.22157;33.08855;,
 7.17363;41.42572;26.90076;,
 32.60313;10.22157;0.00000;,
 26.71820;41.42572;0.00000;,
 -1.56696;1.24649;0.00000;,
 -1.56696;60.17881;0.00000;;
 
 21;
 5;0,1,2,3,4;,
 3;0,4,5;,
 3;4,3,5;,
 3;3,2,5;,
 3;2,1,5;,
 3;1,0,5;,
 4;6,7,8,9;,
 4;7,10,11,8;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,6,9,15;,
 3;7,6,16;,
 3;10,7,16;,
 3;12,10,16;,
 3;14,12,16;,
 3;6,14,16;,
 3;9,8,17;,
 3;8,11,17;,
 3;11,13,17;,
 3;13,15,17;,
 3;15,9,17;;
 
 MeshMaterialList {
  3;
  21;
  2,
  2,
  2,
  2,
  2,
  2,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.401569;0.000000;0.015686;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.501961;0.000000;0.019608;;
  }
  Material {
   0.545882;0.746667;0.800000;0.483607;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;0.327869;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  34;
  0.000000;1.000000;0.000000;,
  0.231845;-0.661135;0.713546;,
  -0.606979;-0.661135;0.440996;,
  -0.606979;-0.661135;0.440995;,
  -0.606979;-0.661135;-0.440996;,
  0.231845;-0.661135;-0.713546;,
  0.231845;-0.661135;-0.713546;,
  0.750267;-0.661135;0.000000;,
  0.658754;-0.580494;0.478613;,
  -0.304399;0.172224;-0.936846;,
  -0.304399;0.172224;-0.936846;,
  -0.427510;0.590538;-0.684471;,
  -0.053563;0.594425;-0.802365;,
  -0.982677;0.185327;0.000000;,
  -0.777715;0.597462;0.195444;,
  -0.982677;0.185327;0.000000;,
  -0.304399;0.172224;0.936846;,
  -0.304399;0.172224;0.936846;,
  -0.053563;0.594425;0.802365;,
  0.799761;0.150831;0.581061;,
  0.799761;0.150831;0.581061;,
  0.735023;0.417801;0.534026;,
  0.799761;0.150831;-0.581061;,
  0.799761;0.150831;-0.581061;,
  0.799761;0.150831;-0.581061;,
  0.735023;0.417801;-0.534026;,
  -0.199735;-0.969157;-0.144366;,
  0.076617;-0.967967;-0.239100;,
  0.005666;-0.999984;0.000000;,
  -0.199735;-0.969157;0.144366;,
  0.076617;-0.967967;0.239100;,
  0.254042;-0.967193;0.000000;,
  -0.404271;0.914639;0.000000;,
  0.552583;0.833458;0.000000;;
  21;
  5;0,0,0,0,0;,
  3;1,2,3;,
  3;2,4,3;,
  3;4,5,6;,
  3;5,7,6;,
  3;7,1,8;,
  4;9,10,11,12;,
  4;13,13,14,15;,
  4;16,17,18,16;,
  4;19,20,21,19;,
  4;22,23,24,25;,
  3;26,27,28;,
  3;29,26,28;,
  3;30,29,28;,
  3;31,30,28;,
  3;27,31,28;,
  3;12,11,32;,
  3;11,14,32;,
  3;14,18,32;,
  3;18,21,33;,
  3;25,12,33;;
 }
}
