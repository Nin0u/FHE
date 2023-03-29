from fractions import Fraction
import random as rd

class Polynomial :
    # Constructeurs
    def __init__(self, n, fill = False, max_coeff = None, fixed_value = None) :
        self.deg = n
        self.coeff = []
        for i in range(n + 1) :
            self.coeff.append(Fraction(0))
        if fill : 
            for i in range(n + 1) :
                self.coeff[i] = Fraction(rd.randint(-max_coeff, max_coeff))
        if fixed_value != None : 
            n = len(fixed_value) -1
            for i in range(len(fixed_value)) :
                self.coeff[i] = fixed_value[i]
    
    # toString
    def __str__(self) : 
        s = ""
        if self.deg >= 0 : s += str(self.coeff[0])
        if self.deg >= 1 : s += " + " + str(self.coeff[1]) + "X"
        for i in range(2, len(self.coeff)) : 
            s +=" + " + str(self.coeff[i]) + "X^" + str(i) 
        return s

    # Copy
    def copy(self) :
        cpy = Polynomial(self.deg)
        for i in range(len(cpy.coeff)) : 
            cpy.coeff[i] = self.coeff[i]
        return cpy

    # Retire les coeffs de plus haut degrés qui valent successivement 0
    # On se ramène ainsi au vrai degré du polynome
    def trim(self) :
        for i in range(len(self.coeff) -1, -1, -1) :
            if (self.coeff[i] != 0) : break
            self.coeff = self.coeff[:i]
            self.deg -= 1
         

    # Getter du coeff de X^i
    def get(self, i) : 
        if(i < len(self.coeff)) : 
            return self.coeff[i]
        else : raise IndexError

    # Check if the Polynomial is Zero
    def isZero(self) :
        for c in self.coeff:
            if c != 0 : return False
        return True

    # Addition
    def add(self, p) :
        result = self.copy()

        if (len(result.coeff) >= len(p.coeff)) : 
            for i in range(len(p.coeff)) :
                result.coeff[i] += p.coeff[i]

        else :
            for i in range(p.deg - result.deg) :
                result.coeff.append(Fraction(0))
            for i in range(len(p.coeff)) :
                result.coeff[i] += p.coeff[i]
            result.deg = p.deg
        result.trim()
        return result

    # Soustraction
    def sub(self, p) :
        for i in range(len(p.coeff)) :
            p.coeff[i] = -p.coeff[i]
        return self.add(p)

    # Multiplication
    def mul(self, p) :
        result = Polynomial(self.deg + p.deg)
        for i in range(len(self.coeff)) :
            p4 = Polynomial(self.deg + p.deg)
            for j in range(len(p.coeff)) :
                p4.coeff[i + j] = self.coeff[i] * p.coeff[j]
            result = result.add(p4)
        result.trim()
        return result

    # Euclidian div par un polynome unitaire
    def euclidianDiv(self, p) :
        if p.isZero() : raise ZeroDivisionError
        q = Polynomial(0)
        r = self.copy()
        d = p.deg
        c = p.coeff[d]

        while((not r.isZero()) and r.deg >= d) : 
            s = Polynomial(r.deg - d)
            s.coeff[r.deg - d] = r.coeff[r.deg] / c
            q = q.add(s)
            s = p.mul(s)
            r = r.sub(s)

        return (q, r)

    # Bezout
    def extendedGCD(self, p):
        r0 = self.copy() 
        r1 = p.copy()
        s0 = Polynomial(0, fixed_value = [1])
        s1 = Polynomial(0, fixed_value = [0]) 
        t0 = Polynomial(0, fixed_value = [0])
        t1 = Polynomial(0, fixed_value = [1])

        i = 1
        while (r1.coeff[0] != 0 or r1.deg != 0) :
            print("r1 =", r1)
            print("deg r1 =", r1.deg)
            q = r0.euclidianDiv(r1)[0]
            qri = q.mul(r1)
            qsi = q.mul(s1)
            qti = q.mul(t1)

            r2 = r0.sub(qri)
            s2 = s0.sub(qsi)
            t2 = t0.sub(qti)

            r0 = r1
            r1 = r2
            s0 = s1
            s1 = s2
            t0 = t1
            t1 = t2

            i += 1
        return r1

# Tests
if __name__ == "__main__" :
    p1 = Polynomial(2, fixed_value= [2,-1,-1])
    p2 = Polynomial(2, fixed_value = [1,1,1])
    print("P1 =", p1)
    print("P2 =", p2)
    print("P1 + P2 =", p1.add(p2))
    print("P1 + P2 - P2 =", p1.add(p2).sub(p2))
    print("P1 * P2 =", p1.mul(p2))
    q,r = p1.euclidianDiv(p2)
    print("P1 = P2*Q + R = (", p2, ") * (", q, ") + (", r, ") =", (p2.mul(q)).add(r))
    print("Bezout : ", p1.extendedGCD(p2))