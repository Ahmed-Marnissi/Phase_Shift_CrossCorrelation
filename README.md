# 📡 Phase Shift Measurement Library

## 📌 Overview  

This project implements a **Phase Shift Measurement** library for STM32 microcontrollers using **CMSIS-DSP**. The library computes the **phase difference** between two periodic signals (e.g., voltage and current) using **cross-correlation**.  

## 🧠 Theory: Measuring Phase Shift Using Cross-Correlation  

When two signals have a phase difference, they appear as **time-shifted** versions of each other. The goal is to **determine this time shift** to compute the phase difference in degrees.  

The **cross-correlation** function finds the similarity between two signals as one is shifted in time. The **peak of the cross-correlation** corresponds to the time shift between the signals:  

 ```
R_{xy}(tau) = sum x(t) y(t+tau)
 ```

Where:  
- \( R_{xy}(tau) \) is the cross-correlation function  
- \( x(t) \) and \( y(t) ) are the two signals  
- \( tau \) is the time shift  

Once the time shift \( t_d \) is found from the correlation peak, the **phase shift** is calculated using:

 ```
theta =  t_d *  f * 360.0 ;
 ```
where  f  is the signal frequency in Hz.

## 🛠 Features  

- ✅ Computes **phase shift** between two signals  
- ✅ Uses **cross-correlation** via **CMSIS-DSP**  
- ✅ Supports **real-time signal processing**  
- ✅ Configurable **sampling frequency**  
- ✅ Compatible with **STM32CubeMonitor** for visualization  

## 🚀 Getting Started  

### 🔹 Requirements  

- **STM32CubeIDE** 
- **CMSIS-DSP Library**  
- **ST-LINK Debugger**  
- **STM32CubeMonitor** for visualization  

### 🔹 Installation  

1. Clone the repository:  
   ```sh
   git clone https://github.com/your-repo/phase-shift-measurement.git

2.Open the project in STM32CubeIDE  

3.Build and flash the firmware to your STM32 device    
## 🛠 Usage
🏗 Initialization

Before using the library, initialize the PhaseShift_Handle_t structure:
 ```
PhaseShift_Handle_t phaseHandle;
 phaseShift_Init(&phaseShiftHandle, SIGNAL_FREQUENCY_HZ , SAMPLE_FREQUENCY_HZ);
 ```

🔄 Real-Time Processing
To process real-time signals, update the phase shift with new samples:
```
float32_t phase = phaseShift_ProcessSample(&phaseHandle, currentSignal, voltageSignal);
```
🎥 CubeMonitor Demo  
Below is a GIF showing the real-time phase shift measurement using STM32CubeMonitor:
![SRM32CubeMonitor Demo](docs/AnimationShift.gif)
🏆 Contribution  
Feel free to contribute by creating pull requests or opening issues.


📜 License  
This project is licensed under the MIT License.

