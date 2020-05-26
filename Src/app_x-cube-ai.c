
#ifdef __cplusplus
 extern "C" {
#endif
/**
  ******************************************************************************
  * @file           : app_x-cube-ai.c
  * @brief          : AI program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2018 STMicroelectronics International N.V.
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "app_x-cube-ai.h"
#include "main.h"
//#include "constants_ai.h"
#include "ai_datatypes_defines.h"

/* USER CODE BEGIN includes */
/* USER CODE END includes */
/* USER CODE BEGIN initandrun */
#include <stdlib.h>

extern UART_HandleTypeDef huart2;

/* Global handle to reference the instance of the NN */
static ai_handle network = AI_HANDLE_NULL;
static ai_buffer ai_input[AI_NETWORK_IN_NUM] = AI_NETWORK_IN ;
static ai_buffer ai_output[AI_NETWORK_OUT_NUM] = AI_NETWORK_OUT ;

/*
 * Init function to create and initialize a NN.
 */
int aiInit(const ai_u8* activations)
{
    ai_error err;

    /* 1 - Specific AI data structure to provide the references of the
     * activation/working memory chunk and the weights/bias parameters */
    const ai_network_params params = {
            AI_NETWORK_DATA_WEIGHTS(ai_network_data_weights_get()),
            AI_NETWORK_DATA_ACTIVATIONS(activations)
    };

    /* 2 - Create an instance of the NN */
    err = ai_network_create(&network, AI_NETWORK_DATA_CONFIG);
    if (err.type != AI_ERROR_NONE) {
	    return -1;
    }

    /* 3 - Initialize the NN - Ready to be used */
    if (!ai_network_init(network, &params)) {
        err = ai_network_get_error(network);
        ai_network_destroy(network);
        network = AI_HANDLE_NULL;
	    return -2;
    }

    return 0;
}

/*
 * Run function to execute an inference.
 */
int aiRun(const void *in_data, void *out_data)
{
    ai_i32 nbatch;
    ai_error err;

    /* Parameters checking */
    if (!in_data || !out_data || !network)
        return -1;

    /* Initialize input/output buffer handlers */
    ai_input[0].n_batches = 1;
    ai_input[0].data = AI_HANDLE_PTR(in_data);
    ai_output[0].n_batches = 1;
    ai_output[0].data = AI_HANDLE_PTR(out_data);

    /* 2 - Perform the inference */
    nbatch = ai_network_run(network, &ai_input[0], &ai_output[0]);
    if (nbatch != 1) {
        err = ai_network_get_error(network);
        // ...
        return err.code;
    }

    return 0;
}
/* USER CODE END initandrun */

/*************************************************************************
  *
  */
void MX_X_CUBE_AI_Init(void)
{
    /* USER CODE BEGIN 0 */
    /* Activation/working buffer is allocated as a static memory chunk
     * (bss section) */
    AI_ALIGNED(4)
    static ai_u8 activations[AI_NETWORK_DATA_ACTIVATIONS_SIZE];

    aiInit(activations);
    /* USER CODE END 0 */
}

int MX_X_CUBE_AI_Process(void)
{
    /* USER CODE BEGIN 1 */
		int res;
	
		float tst[] = {0.15041783,0.24233983,0.34540391,0.50696379,0.57103062,0.62952644,0.73537606,0.81615597,0.80501395,0.82451254,0.82451254,0.83286905,0.83286905,0.84401113,0.83844012,0.8551532,0.85793871,0.87465179,0.88022286,0.89972144,0.89972144,0.92200559,0.91922003,0.93871868,0.93871868,0.96657383,0.96100277,0.97493035,0.96657383,0.98607242,0.98607242,1.,0.99164343,0.99442899,0.98328692,0.98050141,0.96935934,0.95821726,0.94707519,0.93593317,0.90807801,0.90250695,0.88300836,0.88022286,0.8551532,0.86350977,0.84122562,0.84401113,0.82172704,0.82729805,0.81058496,0.81337047,0.79387188,0.79665738,0.7799443,0.77437323,0.76044565,0.76323122,0.75208914,0.75208914,0.74094707,0.74651814,0.72980499,0.74373257,0.72980499,0.74651814,0.71587741,0.71030641,0.7047354,0.72144848,0.7047354,0.71030641,0.69637883,0.7047354,0.68802226,0.7047354,0.69080782,0.69637883,0.68523675,0.70194983,0.69359332,0.7047354,0.68523675,0.69916433,0.68523675,0.69637883,0.67688024,0.71030641,0.69637883,0.71587741,0.71587741,0.7075209,0.66295266,0.64066851,0.54317546,0.45961002,0.27298051,0.,0.06128134,0.17827298,0.31754875,0.45403901,0.52089137,0.632312,0.71309191,0.74651814,0.73537606,0.75208914,0.75487465,0.76601672,0.75766015,0.77437323,0.77437323,0.79944289,0.78551531,0.81337047,0.81058496,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.};

		AI_ALIGNED(4)
		static ai_i8 in_data[AI_NETWORK_IN_1_SIZE_BYTES];

    AI_ALIGNED(4)
    static ai_i8 out_data[AI_NETWORK_OUT_1_SIZE_BYTES];
					
		for (int i=0; i<sizeof(tst); i++){
				((ai_float *) in_data)[i] = (ai_float) tst[i];
		}
	
		/* Perform the inference */
		res = aiRun(in_data, out_data);
		
		ai_float c[5];
		float max = -10.0;
		int max_c = -1;
		for (int i=0; i<5; i++){
			c[i] = ((ai_float *)out_data)[i];
			if(c[i] > max){
				max = c[i];
				max_c = i+1;
			}
		}
		
	 // sprintf(txData, "c: %d\n", max_c);
		//HAL_UART_Transmit(&huart2,(uint8_t *)txData, strlen(txData), HAL_MAX_DELAY);
		/*
		sprintf(txData, "c1: %.10f\n", c[0]);
		HAL_UART_Transmit(&huart2, (uint8_t *)txData, strlen(txData), HAL_MAX_DELAY);
		sprintf(txData, "c2: %.10f\n", c[1]);
		HAL_UART_Transmit(&huart2,(uint8_t *)txData, strlen(txData), HAL_MAX_DELAY);
		sprintf(txData, "c3: %.10f\n", c[2]);
		HAL_UART_Transmit(&huart2,(uint8_t *)txData, strlen(txData), HAL_MAX_DELAY);
		sprintf(txData, "c4: %.10f\n", c[3]);
		HAL_UART_Transmit(&huart2,(uint8_t *)txData, strlen(txData), HAL_MAX_DELAY);
		sprintf(txData, "c5: %.10f\n", c[4]);
		HAL_UART_Transmit(&huart2,(uint8_t *)txData, strlen(txData), HAL_MAX_DELAY);
		*/
		return max_c;
    /* USER CODE END 1 */
}
#ifdef __cplusplus
}
#endif
