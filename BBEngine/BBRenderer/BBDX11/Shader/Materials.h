#pragma once
#include "ShaderDefines.h"

cbuffer MaterialConstant CBRegister2
{
    float4 baseColor;                   //16
    int hasTexture;                     //20

    int hasKhrTransmission;             //24
    int hasKhrTransmissionTexture;      //28
    float khrTransmissionFactor;        //32

    int hasKhrVolume;                   //36
    int hasKhrVolumeTexture;            //40
    float khrThicknessFactor;           //44
    float khrAttenuationDistance;       //48
    float3 khrAttenuationColor;         //60
    float pad;                          //64
};