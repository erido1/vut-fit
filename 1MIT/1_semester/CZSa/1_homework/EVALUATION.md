1a)  
Used the buggy version of quantize. The bug is of course not your fault, but removing the assertion statement was not ideal. (No reduction)  
**Points:** 1p

---

1b)  
OK, I did not fully understand the second point. "The second graph shows that the error distribution is not perfectly uniform but gradually converges toward uniformity."  
  — I guess this point is intended for 1c.  
**Points:** 0.75p

---

1c)  
Correct analysis for the signal. The analysis for the error was found in 1b.  
It is correct, but it would have been desirable to see the result for different numbers of bits.  
**Points:** 0.75p

---

2a)  
Correct.  
**Points:** 1p

---

2b)  
The implementation looks generally correct. A minor point:  
In the following code:  
`sample_times = np.linspace(0, len(sgn_cut) / r, length_rec_sgn)`  
— I think "r/4" should have been used instead of "r". However, this is compensated by the fact that you also send "r" instead of "r/4" to your sinc_interpolation function.  
So, two "errors" that cancel out.  
Linear and sinc interpolation is compared, and the figures seem to show that sinc interpolation is more accurate. The conclusion that 32 samples is sufficient is reasonable, but how did you reach this conclusion? By looking at the figures? In general, it may be better to measure the accuracies somehow in addition to looking at figures.  
**Points:** 0.75p

---

3b)  
The statement:  
  "The coefficients h [k] are nonzero exclusively for k values in the range -N, -(N−1),…,0,…,N−1,N."  
  — This is not correct. It should be for k = 0,..,2N. The summation start and end index in Eq. 7 is accordingly wrong. However, in Eq. 8 you get it right, except that here, h[0] occurs twice. To get it right, you would have to include h[n]e^{-jwN} as a separate term and start the sum from n=1.

The bullets below Eq. 8 are slightly incorrect, e.g., the first term does not correspond to negative values of k but to k=0,N.  
  In any case, these are minor issues that do not affect the main idea of the derivation.  
**Points:** 1.5p

---

**Total Score:**  
`1 + 0.75 + 0.75 + 1 + 0.75 + 1.5 = 5.75p`
