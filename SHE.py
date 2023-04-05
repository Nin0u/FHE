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
"""
    Classe représentant le SHE
     Attribut :
        - polMod : le polynome X^n + 1 où n est une puissance de 2
        - deg : le degré de polMod qui est aussi la dimension du réseau

        - v : le polynome v(x) qui est la clé secrete
        - w : le polynome w(x) qui est aussi la clé secrete
        - d de la HNF clé publique
        - r de la HNF clé publique
"""
class SHE :
    # Constructeur
    def __init__(self, deg) :
        self.polMod = Polynomial(1 << deg)
        self.polMod.coeff[0] = 1
        self.polMod.coeff[1 << deg] = 1
        self.deg = 1 << deg
        
    # =========== GENERATION DES CLES =========== #
    def keyGenAux(self) :
        self.v = Polynomial(self.deg - 1, True, 1 << 10)
        G, U, V = self.polMod.bezoutInt(self.v)
        # U(X) * (X^n + 1) + v(x) * V(X) = G
        
        if(G.deg != 0) : return False
        d = G.coeff[0]
        if d < 0 :
            d *= -1
            V = V.mul(Polynomial(0, fixed_value=[-1]))
        
        if(d % 2 == 0) : return False
        
        oddCoeff = False
        for i in range(len(V.coeff)) :
            if V.coeff[i]  % 2 == 1 :
                oddCoeff = True
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
        self.wi = V.coeff[i]
        return True
    
    def keyGen(self) :
        while(not crypto.keyGenAux()) :
            a = 0
    
    # Encryption
    def Encrypt(self, bit) :
        # On convertit le bit a encoder en polynome
        b = Polynomial(self.deg - 1, fixed_value=[bit])
        
        # Calcul de 2u 
        u = Polynomial(self.deg - 1, fill = True, max_coeff = 1)
        u = u.mul(Polynomial(0, fixed_value=[2]))
        
        a = b.add(u)
        c = a.eval(self.r, self.d)
        if(c > self.d // 2) : c -= self.d
        return c
        
    # Decryption
    def Decrypt(self, ciphertext) :
        wi = self.wi
        m = (wi * ciphertext) % self.d
        if(m >= self.d // 2) : m -= self.d
        b = m % 2
        return b
    
    def EncryptBis(self, bit) :
        res = []
        ciphertext = self.Encrypt(bit)
        res.append(ciphertext)
        for i in range(len(self.t)) :
            res.append(ciphertext * self.t[i])
        return res
    
    def ExpandCipher(self, cipher) :
        res = []
        res.append(cipher)
        for i in range(len(self.t)) :
            res.append(cipher * self.t[i])
        return res
    
    def DecryptBis(self, cipher) :
        m = 0
        for i in range(len(self.t)) :
            m += cipher[i + 1] * self.sigma[i]
        m %= self.d
        if(m >= self.d // 2) : m -= self.d
        return m % 2
    
    def SplitKey(self) :
        t1 = rd.randint(-self.d//2,self.d//2)
        t2 = rd.randint(-self.d//2,self.d//2)
        t3 = self.wi - t1
        self.t = []
        self.t.append(t1)
        self.t.append(t2)
        self.t.append(t3)
        self.sigma = []
        self.sigma.append(1)
        self.sigma.append(0)
        self.sigma.append(1)
        
    def Recrypt(self, cipher) :
        cryptoKey = []
        for i in range(len(self.sigma)):
            cryptoKey.append(self.Encrypt(self.sigma[i]))
        
        m = 0
        for i in range(len(self.t)) :
            m += cipher[i + 1] * cryptoKey[i]
        m %= self.d
        if(m >= self.d // 2) : m -= self.d
        return self.ExpandCipher(m)
                
if __name__ == "__main__":
    crypto = SHE(8) # X^4 + 1
    crypto.keyGen()
    print("V =", crypto.v)
    print("W =", crypto.w)
    print("dd =", crypto.v.mul(crypto.w).euclidianDivInt(crypto.polMod)[2])
    print("d =", crypto.d)
    print("r =", crypto.r)
    print("wi =", crypto.wi)
    
    print("############## TEST ENCRYPT/DECRYPT ############")
    bit1 = rd.randint(0, 1)
    bit2 = rd.randint(0, 1)
    print("bit1 =", bit1)
    print("bit2 =", bit2)
    ciphertext1 = crypto.Encrypt(bit1)
    ciphertext2 = crypto.Encrypt(bit2)
    print("c1 =", ciphertext1)
    print("c2 =", ciphertext2)
    
    b1 = crypto.Decrypt(ciphertext1)
    b2 = crypto.Decrypt(ciphertext2)
    b3 = crypto.Decrypt(ciphertext1 + ciphertext2)
    b4 = crypto.Decrypt(ciphertext1 * ciphertext2)
    print("b1 =", b1)
    print("b2 =", b2)
    print("b1 + b2 =", b3)
    print("b1 * b2 =", b4)
    print()
    
    print("############## TEST ENCRYPT/DECRYPT BIS ############")
    crypto.SplitKey()
    bit1 = rd.randint(0, 1)
    bit2 = rd.randint(0, 1)
    print("bit1 =", bit1)
    print("bit2 =", bit2)
    ciphertext1 = crypto.EncryptBis(bit1)
    ciphertext2 = crypto.EncryptBis(bit2)
    print("c1 =", ciphertext1)
    print("c2 =", ciphertext2)
    
    b1 = crypto.DecryptBis(ciphertext1)
    b2 = crypto.DecryptBis(ciphertext2)
    ciphertextSum = [ciphertext1[i] + ciphertext2[i] for i in range(len(ciphertext1))]
    b3 = crypto.DecryptBis(ciphertextSum)
    ciphertextMul = crypto.ExpandCipher(ciphertext1[0] * ciphertext2[0])
    # ciphertextMul = [ciphertext1[i] * ciphertext2[i] for i in range(len(ciphertext1))]
    b4 = crypto.DecryptBis(ciphertextMul)
    print("b1 =", b1)
    print("b2 =", b2)
    print("b1 + b2 =", b3)
    print("b1 * b2 =", b4)
    print()
    
    #! Question comment rencrypter la clé secrete, ie les t[i] !
    # print("############## TEST RECRYPT ############")
    # bit1 = rd.randint(0, 1)
    # print("bit1 =", bit1)
    # ciphertext1 = crypto.EncryptBis(bit1)
    # ciphertext2 = crypto.Recrypt(ciphertext1)
    # print(ciphertext1)
    # print(ciphertext2)
    # b1 = crypto.DecryptBis(ciphertext1)
    # b2 = crypto.DecryptBis(ciphertext2)
    
    # print("b1 =", b1)
    # print("b2 =", b2)
    