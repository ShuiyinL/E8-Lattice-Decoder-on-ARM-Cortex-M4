# E8-Lattice-Decoder-on-ARM-Cortex-M4 and Decryption Failure Attack Estimator
This repository evaluates the clock cycle performance of E8 lattice decoder [1] on an ARM Cortex-M4 processor using the STM32L412KB (NUCLEO-L412KB, Cortex-M4 at 80 MHz) development board.
- For reference, we have also provided the implementation on the ARM Fixed Virtual Platform (FVP): ARM_FVP_E8DecoderCycleCount.c.

The decryption failure attack estimation for P_l Kyber are also included. The software used for this analysis [2] is available in: https://github.com/KULeuven-COSIC/PQCRYPTO-decryption-failures/tree/master/DecryptionFailureAttack
- copy "P_Kyber_failureboost.py", "Parameters_P_Kyber.py", "Cv4.mat", and "Cv5.mat" to the folder "DecryptionFailureAttack"
- run "P_Kyber_failureboost.py"
- For reference, the precomputed values of α and β are stored in the files named "P_KyberXXXX.pkl" or "KyberXXXX.pkl"

MATLAB codes for MMSE quantization are also provided. MMSE quantization can be used to replace the original Kyber quantization function, as it yields a smaller quantization noise variance [3]. 
- run "MMSE_Quantization.m"
- For comparison, the performance of the original Kyber quantization is also tested in "MMSE_Quantization.m"


[1] Conway, J., Sloane, N.: Fast quantizing and decoding and algorithms for lattice quantizers and codes. IEEE Transactions on Information Theory 28(2), 227–232 (1982). https://doi.org/10.1109/TIT.1982.1056484

[2] D’Anvers, JP., Guo, Q., Johansson, T., Nilsson, A., Vercauteren, F., Verbauwhede, I. (2019). Decryption Failure Attacks on IND-CCA Secure Lattice-Based Schemes. In: Lin, D., Sako, K. (eds) Public-Key Cryptography – PKC 2019. PKC 2019. Lecture Notes in Computer Science(), vol 11443. Springer, Cham. https://doi.org/10.1007/978-3-030-17259-6_19

[3] Liu, S., Sakzad, A. (2025). Semi-compressed CRYSTALS-Kyber. In: Liu, J.K., Chen, L., Sun, SF., Liu, X. (eds) Provable and Practical Security. ProvSec 2024. Lecture Notes in Computer Science, vol 14904. Springer, Singapore. https://doi.org/10.1007/978-981-96-0957-4_4
