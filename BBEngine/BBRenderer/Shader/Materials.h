#pragma once
#include "ShaderDefines.h"

cbuffer MaterialConstant CBRegister2
{
    //PBR
    float4 baseColor;                   //16
    float metallicFactor;               //20
    float roughnessFactor;              //24
    int hasBaseColorTexture;            //28
    int hasMetallicRoughnessTexture;    //32

    //Normal
    int hasNormalTexture;               //36
    float normalScale;                  //40

    //Occlusion
    int hasOcclusionTexture;            //44
    float occlusionStrenght;            //48

    //Emission
    int hasEmissionTexture;             //52
    float4 emmisiveFactor;              //68

    //KHR Transmission
    int hasKhrTransmission;             //72
    int hasKhrTransmissionTexture;      //76
    float khrTransmissionFactor;        //80

    //KHR Volume
    int hasKhrVolume;                   //84
    int hasKhrVolumeTexture;            //88
    float khrThicknessFactor;           //92
    float khrAttenuationDistance;       //96
    float3 khrAttenuationColor;         //108
    float pad;                          //112
};