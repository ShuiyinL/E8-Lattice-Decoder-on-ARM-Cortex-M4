# E8-Lattice-Decoder-on-ARM-Cortex-M4 and Decryption Failure Attack Estimator
This repository evaluates the clock cycle performance of E8 lattice decoder [1] on an ARM Cortex-M4 processor using the STM32L412KB (NUCLEO-L412KB, Cortex-M4 at 80 MHz) development board.
- For reference, we have also provided the implementation on the ARM Fixed Virtual Platform (FVP): ARM_FVP_E8DecoderCycleCount.c.

The decryption failure attack estimation for P_l Kyber are also included. The software used for this analysis [2] is available in: https://github.com/KULeuven-COSIC/PQCRYPTO-decryption-failures/tree/master/DecryptionFailureAttack
- copy "P_Kyber_failureboost.py", "Parameters_P_Kyber.py", "Cv4.mat", and "Cv5.mat" to the folder "DecryptionFailureAttack"
- run "P_Kyber_failureboost.py"
- For refetrence, the precomputed alpha and beta are stored in "P_KyberXXXX.pkl" or "KyberXXXX.pkl" file


[1] Conway, J., Sloane, N.: Fast quantizing and decoding and algorithms for lattice quantizers and codes. IEEE Transactions on Information Theory 28(2), 227–232 (1982). https://doi.org/10.1109/TIT.1982.1056484

[2] D’Anvers, JP., Guo, Q., Johansson, T., Nilsson, A., Vercauteren, F., Verbauwhede, I. (2019). Decryption Failure Attacks on IND-CCA Secure Lattice-Based Schemes. In: Lin, D., Sako, K. (eds) Public-Key Cryptography – PKC 2019. PKC 2019. Lecture Notes in Computer Science(), vol 11443. Springer, Cham. https://doi.org/10.1007/978-3-030-17259-6_19
