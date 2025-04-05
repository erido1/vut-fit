1a)  
Correct computations.  
**Points:** 1p

---

1b)  
**What is the best value of the e_test?**  
  — The answer seems to be for sigma, not for e_test. However, the correct answer was printed by the code.

**How does e_train and e_test differ?**  
  (Consider also setting the order to something large like 200.)  
  — Good discussion in general.  
  (It is questionable whether large e_train compared to e_test is a property of overfitting, but this can be seen as a matter of terminology.)

**How do σ and e differ, and why?**  
  — Good discussion.

**What do you think is the order of the true filter and what are its parameters?**  
  — Correct.  
**Points:** 2.0p

---

1c)  
**Why did you use `* 2` in this expression:**  
  `fft_chunk = np.fft.fft(chunk, n=chunk_length * 2)`  
  — It doesn't harm, but you need to consider that you'll get twice as many frequencies in the DFT output.  
  So, you should use:  
  `freqs = np.fft.fftfreq(chunk_length * 2)`  
  instead of:  
  `freqs = np.fft.fftfreq(chunk_length)`  
  — Currently, the frequency axis in your plot has the wrong scale.

**Regarding this line:**  
  `h_U = signal.lfilter(b, a, u_test)  # U(z) * a_k / b_k`  
  — The comment should read: `# U(z) * b_k / a_k` to match the actual operation.

**Recreating `e`:**  
  — Very creative and clever approach.  
  However, theoretically, to invert a filter operation, you should flip the roles of `a_k` and `b_k`.  
  That is, to recover `e`, use:  
  `e = signal.lfilter(a, 1, e_filtered)`  
  assuming that:  
  `e_filtered = signal.lfilter(1, a, e)`  
  — These filters’ transfer functions cancel each other out in the z-domain.  
  — Note: this approach might not be robust in practice. Small perturbations between filtering steps can affect the result.  
  A more robust solution in such cases would be to use a **Wiener filter**.

**Points:** 1.75p

---

&nbsp;2)  
Correct.  
**Points:** 2p  
(Small note: not sure what "pointwise product" refers to in this context.)

---

**Total Score:**  
`1 + 2 + 1.75 + 2 = 6.75`
