/**
 *******************************************************************************
 * @file    OpenPDMFilter.h
 * @author  CL
 * @version V1.0.0
 * @date    9-September-2015
 * @brief   Header file for Open PDM audio software decoding Library.   
 *          This Library is used to decode and reconstruct the audio signal
 *          produced by ST MEMS microphone (MP45Dxxx, MP34Dxxx). 
 *******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2018 STMicroelectronics</center></h2>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __OPENPDMFILTER_H
#define __OPENPDMFILTER_H

#ifdef __cplusplus
  extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/

#include <stdint.h>


/* Definitions ---------------------------------------------------------------*/

/*
 * Enable to use a Look-Up Table to improve performances while using more FLASH
 * and RAM memory.
 * Note: Without Look-Up Table up to stereo@16KHz configuration is supported.
 */
//#define USE_LUT

#define SINCN            3
#define DECIMATION_MAX 128

#define HTONS(A) ((((uint16_t)(A) & 0xff00) >> 8) | \
                 (((uint16_t)(A) & 0x00ff) << 8))
#define RoundDiv(a, b)    (((a)>0)?(((a)+(b)/2)/(b)):(((a)-(b)/2)/(b)))
#define SaturaLH(N, L, H) (((N)<(L))?(L):(((N)>(H))?(H):(N)))


/* Types ---------------------------------------------------------------------*/

typedef struct {
  /* Public */
  float LP_HZ;
  float HP_HZ;
  uint16_t Fs;
  unsigned int nSamples;
  uint8_t In_MicChannels;
  uint8_t Out_MicChannels;
  uint8_t Decimation;
  uint8_t MaxVolume;
  /* Private */
  uint32_t Coef[SINCN];
  uint16_t FilterLen;
  int64_t OldOut, OldIn, OldZ;
  uint16_t LP_ALFA;
  uint16_t HP_ALFA;
  uint16_t bit[5];
  uint16_t byte;
  uint16_t filterGain;
} TPDMFilter_InitStruct;


/* Exported functions ------------------------------------------------------- */

void Open_PDM_Filter_Init(TPDMFilter_InitStruct *init_struct);
void Open_PDM_Filter_64(uint8_t* data, int16_t* data_out, uint16_t mic_gain, TPDMFilter_InitStruct *init_struct);
void Open_PDM_Filter_128(uint8_t* data, int16_t* data_out, uint16_t mic_gain, TPDMFilter_InitStruct *init_struct);

#ifdef __cplusplus
}
#endif

#endif // __OPENPDMFILTER_H

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
