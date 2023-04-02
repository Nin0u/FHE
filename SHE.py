from Polynomial import *
import math


def xgcd(a,b):
    prevx, x = 1, 0;  prevy, y = 0, 1
    while b:
        q, r = divmod(a,b)
        x, prevx = prevx - q*x, x  
        y, prevy = prevy - q*y, y
        a, b = b, r
    return a, prevx, prevy




# Attribut :
# le polynome X^n + 1 où n est une puissance de 2
# le polynome v(x) la clé secrete
# le polynome w(x) aussi la clé secrete
# d de la HNF clé publique
# r de la HNF clé publique
class SHE :
    def __init__(self, deg) :
        self.polMod = Polynomial(1 << deg)
        self.polMod.coeff[0] = 1
        self.polMod.coeff[1 << deg] = 1
        self.deg = 1 << deg
        
    def keyGenAux(self) :
        self.v = Polynomial(self.deg - 1, True, 1 << 10)
        G, U, V = self.polMod.bezoutInt(self.v)
        # U(X) * (X^n + 1) + v(x) * V(X) = G
        
        if(G.deg != 0) : return False
        d = G.coeff[0]
        if d < 0 :
            d *= -1
            V.mul(Polynomial(0, fixed_value=[-1]))
        
        if(d % 2 == 0) : return False
        
        oddCoeff = False
        for i in range(len(V.coeff)) :
            if V.coeff[i]  % 2 == 1 :
                oddCoeff = 1
                break
        if not oddCoeff : return False
        
        g, v1, v2 = xgcd(V.coeff[1], d)
        if(g != 1) : return False
        r = (V.coeff[0] * v1) % d
        res = (pow(r, self.deg, d) + 1) % d
        if(res != 0) : return False
        
        self.d = d
        self.r = r
        self.w = V
        return True
    
    def keyGen(self) :
        while(not crypto.keyGenAux()) :
            a = 0
    
    def Encrypt(self, bit) :
        b = Polynomial(self.deg - 1, fixed_value=[bit])
        for i in range(1, self.deg) :
            b.coeff[i] = 0
        
        u = Polynomial(self.deg - 1)
        for i in range(self.deg) :
            u.coeff[i] = rd.randint(-1, 1)
        
        
        u = u.mul(Polynomial(0, fixed_value=[2]))
        
        a = b.add(u)
        c = a.eval(self.r, self.d) % self.d
        if(c > self.d / 2) : c -= self.d
        return c
        
    
    def Decrypt(self, ciphertext) :
        wi = 0
        for i in range(self.w.deg + 1) :
            if(self.w.coeff[i] % 2 == 1) :
                wi = self.w.coeff[i]
                break
        m = (wi * ciphertext) % self.d
        if(m >= self.d / 2) : m -= self.d
        b = m % 2
        return b
                
if __name__ == "__main__":
    crypto = SHE(3) # X^4 + 1
    crypto.keyGen()
    print("V =", crypto.v)
    print("W =", crypto.w)
    print("dd =", crypto.v.mul(crypto.w).euclidianDivInt(crypto.polMod)[2])
    print("d =", crypto.d)
    print("r =", crypto.r)
    bit = rd.randint(0, 1)
    print("bit =", bit)
    ciphertext = crypto.Encrypt(bit)
    print("c =", ciphertext)
    b = crypto.Decrypt(ciphertext)
    print("b =", b)