<h2>ECG Heart monitor using the STM32 Nucleo Board and TensorFlow Lite</h2>
Both the Report and the Presentation explaining the Project in great details are found in the Report_Presentation Folder.
<br>
<h3>Documentation</h3>
Below is the documentation for the most importatnt functions. More documentation is found inside the code itself.
<br>
Convention: functionName : parameter1, parameter2, ... : return value1, return value2, ... <br>
<h4>Main</h4>
<ul>
<li> HAL_TIM_PeriodElapsedCallback : void : void </li><br>
Callback whenever the Timer TIM15 raises an interrupt (when the timer == value AR register)
This is where the ADC performs the conversion.

<li> main : no arguments : returns void (never returns) </li><br>
Check if ready then performs inference and outputs the result using UART2.
</ul>
<h4>app_x-cube-ai.c</h4>
<ul>
<li> aiInit : ai_u8 activations : void </li><br>
Initializes the Neural Network

<li> aiRun : const void * in_data (input float buffer with the 187 ECG samples), void * out_data (output int buffer of size 5 (5 classes)) : int (prediction status) </li><br>
Performs the Inference

<li> MX_CUBE_AI_Init : void : void </li><br>
Calls aiInit

<li> MX_CUBE_AI_Process : void : int (the prediction class) </li><br>
Initializes the input buffer by converting the ECG buffer to bytes then feeding it to aiRun which outputs the prediction then the predicted class is returned
</ul>
