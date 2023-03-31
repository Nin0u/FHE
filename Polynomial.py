import random as rd
import math as mat

class Polynomial :
    # Constructeurs
    def __init__(self, n, fill = False, max_coeff = None, fixed_value = None) :
        self.deg = n
        self.coeff = []
        for i in range(n + 1) :
            self.coeff.append(0)
        if fill : 
            for i in range(n + 1) :
                self.coeff[i] = rd.randint(-max_coeff, max_coeff)
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
                result.coeff.append(0)
            for i in range(len(p.coeff)) :
                result.coeff[i] += p.coeff[i]
            result.deg = p.deg
        result.trim()
        return result

    # Soustraction
    def sub(self, p) :
        s = p.copy()
        for i in range(len(s.coeff)) :
            s.coeff[i] = -s.coeff[i]
        return self.add(s)

    # Multiplication
    def mul(self, p) :
        result = Polynomial(self.deg + p.deg)
        for i in range(len(self.coeff)) :
            p4 = Polynomial(self.deg + p.deg)
            for j in range(len(p.coeff)) :
                p4.coeff[i + j] = self.coeff[i] * p.coeff[j]
            result = result.add(p4)
        return result
    
    def div(self, elt) :
        for i in range(len(self.coeff)) :
            self.coeff[i] //= elt
    
    def contenu(self) :
        pgcd = 0
        for i in range(len(self.coeff)) :
            pgcd = mat.gcd(pgcd, self.coeff[i])
        return pgcd

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

    def bezout(self, p) : 
        r1 = self.copy()
        r2 = p.copy()
        u1 = Polynomial(0, fixed_value = [1])
        u2 = Polynomial(0, fixed_value = [0])
        v1 = Polynomial(0, fixed_value = [0])
        v2 = Polynomial(0, fixed_value = [1])

        while(not r2.isZero()) :
            q = r1.euclidianDiv(r2)[0]
            rs = r1 
            us = u1
            vs = v1 

            r1 = r2
            u1 = u2
            v1 = v2

            r2 = rs.sub(q.mul(r2))
            u2 = us.sub(q.mul(u2))
            v2 = vs.sub(q.mul(v2))

        return (r1,u1,v1)
    
    def bezoutInt(self, p) : 
        r1 = self.copy()
        r2 = p.copy()
        u1 = Polynomial(0, fixed_value = [1])
        u2 = Polynomial(0, fixed_value = [0])
        v1 = Polynomial(0, fixed_value = [0])
        v2 = Polynomial(0, fixed_value = [1])

        while(not r2.isZero()) :
            d,q,r = r1.euclidianDivInt(r2)
            D = Polynomial(0, fixed_value=[d])
            rs = r1.mul(D)
            us = u1.mul(D)
            vs = v1.mul(D) 
            
            r1 = r2
            u1 = u2
            v1 = v2

            r2 = rs.sub(q.mul(r2))
            u2 = us.sub(q.mul(u2))
            v2 = vs.sub(q.mul(v2))
            
            pgcd = mat.gcd(r2.contenu(), mat.gcd(u2.contenu(), v2.contenu()))
            if(pgcd != 0) :
                r2.div(pgcd)
                u2.div(pgcd)
                v2.div(pgcd)

        return (r1,u1,v1)
    
     # Euclidian div par un polynome coeff entier
    def euclidianDivInt(self, p) :
        if p.isZero() : raise ZeroDivisionError
        q = Polynomial(0)
        r = self.copy()
        m = self.deg
        n = p.deg
        d = p.coeff[n]
        D = Polynomial(0, fixed_value=[d])
        e = m - n + 1
        
        while((not r.isZero()) and r.deg >= n) : 
            s = Polynomial(r.deg - n)
            s.coeff[r.deg - n] = r.coeff[r.deg]
            q = q.mul(D).add(s)
            r = r.mul(D).sub(s.mul(p))
            e -= 1
        # print("OUF =",q.mul(p).add(r))
        d = pow(d, e)
        D = Polynomial(0, fixed_value=[d])
        q = q.mul(D)
        r = r.mul(D)
        d = pow(p.coeff[n], m - n + 1)
        return (d, q, r)

# Tests
if __name__ == "__main__" :
    p1 = Polynomial(10, True, 10)
    p2 = Polynomial(5, True, 10)
    print("P1 =", p1)
    print("P2 =", p2)
    # print("P1 + P2 =", p1.add(p2))
    # print("P1 + P2 - P2 =", p1.add(p2).sub(p2))
    # print("P1 * P2 =", p1.mul(p2))
    # q,r = p1.euclidianDiv(p2)
    # print("P1 = P2*Q + R = (", p2, ") * (", q, ") + (", r, ") =", (p2.mul(q)).add(r))
    # r,u,v = p1.bezout(p2)
    # print("R = P1*U + P2*V = (", p1, ")*(", u, ") + (", p2, ")*(", v, ") =", (p1.mul(u)).add(p2.mul(v)))
    d, q, r = p1.euclidianDivInt(p2)
    D = Polynomial(0, fixed_value=[d])
    print(d, q, r)
    print(p2.mul(q).add(r))
    print(p1.mul(D))
    
    r, u, v = p1.bezoutInt(p2)
    print("R =",r, "U =",u, "V =",v)
    print("P1 * U + P2 * V =",(p1.mul(u)).add(p2.mul(v)))