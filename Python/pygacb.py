#!/usr/bin/python3

import abc
import random
import operator

#
# Genetic algorithm
#
class GeneticAlgorithm:
    def __init__(self,
            solution_cls : "class of solutions",
            data : "any data to pass to solution",
            input : "input of the algorithm",
            population_size : "number of solution in a generation",
            birth_rate : "rate of new solution created from merges",
            immigration_rate : "rate of new solution created randomly"
        ):
        self.generation = 0
        self.solution_cls = solution_cls
        self.data = data
        self.input = input
        self.birth_rate = birth_rate
        self.immigration_rate = immigration_rate
        self.best = solution_cls(self.input, self.data)
        self.population = []
        # init random population
        for i in range(0, population_size):
            c = self.solution_cls(self.input, data)
            self.bestify(c)
            self.population.append(c)
            
    def evolve(self):
        """Evolve current generation using reproduction and immigration"""
        self.generation += 1
        self.best.age += 1
        for i in range(0, len(self.population)):
            self.population[i].age += 1
        birth = round(self.birth_rate * len(self.population))
        immigration = round(self.immigration_rate * len(self.population))
        # sort
        self.population = sorted(self.population, key=operator.attrgetter('score'), reverse=True)
        # reproduce
        for i in range(0, birth):
            a = self.select()
            b = self.select()
            c = a.merge(self.input, b)
            self.bestify(c)
            self.population.append(c)
        # immigrant
        for i in range(0, immigration):
            c = self.solution_cls(self.input, self.data)
            self.bestify(c)
            self.population.append(c)
        # sort
        self.population = sorted(self.population, key=operator.attrgetter('score'), reverse=False)
        # die
        for i in range(0, birth+immigration):
            self.population.remove(self.selectInvert())
            
    def select(self):
        """Select a solution.
        The higher the score, the higher the chance
        of being selected in this method.
        The population need to be ordered in a decreasing order"""
        p = .30
        while 1:
            p += .05
            for i in range(0, len(self.population)):
                if random.uniform(0,1) < p*self.population[i].score:
                    return self.population[i]
        
    def selectInvert(self):
        """Select a solution.
        The lower the score, the higher the chance
        of being selected in this method.
        The population need to be ordered in an increasing order"""
        p = .40
        while 1:
            p += .05
            for i in range(0, len(self.population)):
                if random.uniform(0,1) < p*(1-self.population[i].score):
                    return self.population[i]
    
    def bestify(self, s : "checked solution"):
        """Check if the given parameter is better than best,
        and change best if necessary"""
        if s.score > self.best.score:
            self.best = s
        

#
# Solution (proposal)
#
class Solution:    
    __metaclass__ = abc.ABCMeta
    
    def __init__(self):
        self.age = 0
        self.score = None
    
    @abc.abstractmethod
    def eval(self, input):
        """Evaluate this against input (should return a value between 0 and 1)"""
        return
    
    @abc.abstractmethod
    def merge(self, ct, b):
        """Create merged solution (child)"""
        return
    

#
# Solution for vigenere cipher
#
class VigenereSolution(Solution):
    # create a new "solution" for the ciphered text *ct* and for data *data*
    # data is a dic containing 3 keys: charset, language and vigenere
    def __init__(self,
            ct : "ciphered text" ,
            data : "dic containing charset, language and vigenere-object"
        ):
        super().__init__()
        self.data = data
        self.charset = data['charset']
        self.language = data['language']
        self.vigenere = data['vigenere']
        self.key = self.random()
        self.score = self.eval(ct)
    
    # decipher the given text using self.key, then
    # return a score between 0 (unlikely) to 1 (likely)
    # for the result to be a valid text in language
    def eval(self, text : "ciphered text"):
        deciphered = self.vigenere.decode(text, self.key)
        return self.simpleEval(deciphered)
    
    # return a score between 0 (unlikely) to 1 (likely)
    # for a text to be a valid text in language
    def simpleEval(self, text : "deciphered text"):
        score = 0;
        coeff = 0
        for k in self.language.keys():
            f = self.language[k]
            if f < 1:
                fc = self.frequency(text, k)
                score += 1 - abs(f - fc)/max(f, fc)
                coeff += 1
            else:
                c = text.count(k)
                if c > 0:
                    score += f*c
                    coeff += f*c
        return score/coeff
    
    # get frequency of a given chunck in the global text
    def frequency(self, text, chunck):
        t = text.replace(" ", "")
        if len(t) == 0:
            return 0
        return t.count(chunck)*len(chunck)/len(t) 
    
    # create random key
    def random(self):
        result = ""
        min_size = 1
        max_size = 10
        size = random.randrange(min_size, max_size)
        for i in range(0, size):
            result += self.charset[ random.randrange(0, len(charset)) ]
        return result
    
    # merge two solution (this and b)
    def merge(self, ct : "ciphered text (for scoring)", b : "other solution"):
        k_max = max(len(self.key), len(b.key))
        a_s = len(self.key)
        b_s = len(b.key)
        c = VigenereSolution("", self.data)
        c.key = ""
        for i in range(0, k_max):
            while len(c.key) <= i:
                r = random.randrange(0, 100)
                if r < 50 and i < a_s:
                    c.key += self.key[i]
                elif r < 98 and i < b_s:
                    c.key += b.key[i]
                elif i > 0:
                    break
        c.score = c.eval(ct)
        return c
    
    # pretty print
    def __str__(self):
        "{} (score: {}, age: {})".format(self.key, self.score, self.age)

#
# Vigenere
#
class Vigenere:
    def __init__(self, charset):
        self.charset = charset
    
    def decode(self, text, key):
        """Cipher a text using a key"""
        return self.run(text, key, -1)
    
    def encode(self, text, key):
        """Decipher a text using a key"""
        return self.run(text, key, +1)
    
    def run(self, text, key, d : "1 for cipher, -1 for decipher"):
        """Vigenere cipher/decipher"""
        result=""
        for i in range(0, len(text)):
            k = key[i % len(key)]
            k = self.charset.find(k)
            k *= d
            t = self.charset.find(text[i])
            if t >= 0:
                result += self.charset[(t+k) % len(self.charset)]
            else:
                result += text[i]
        return result
        


#
# French frequency map
#
french = {
    'e' : 0.1587,
    'a' : 0.0942,
    'i' : 0.0841,
    's' : 0.0790,
    't' : 0.0726,
    'n' : 0.0715,
    'r' : 0.0646,
    'u' : 0.0624,
    'l' : 0.0534,
    'o' : 0.0514,
    'd' : 0.0339,
    'm' : 0.0324,
    'p' : 0.0286,
    'c' : 0.0264,
    'v' : 0.0215,
    'q' : 0.0106,
    'g' : 0.0104,
    'b' : 0.0102,
    'f' : 0.0095,
    'j' : 0.0089,
    'h' : 0.0077,
    'z' : 0.0032,
    'x' : 0.0030,
    'y' : 0.0024,
    'k' : 0.0005,
    'w' : 0.0004,
    'es' : 3318/100000,
    'de' : 2409/100000,
    'le' : 2366/100000,
    'en' : 2121/100000,
    're' : 1885/100000,
    'nt' : 1694/100000,
    'on' : 1646/100000,
    'er' : 1514/100000,
    'te' : 1484/100000,
    'el' : 1382/100000,
    'an' : 1378/100000,
    'se' : 1377/100000,
    'et' : 1307/100000,
    'la' : 1270/100000,
    'ai' : 1255/100000,
    'it' : 1243/100000,
    'ent' : 84/100000,
    'que' : 69/100000,
    'les' : 66/100000,
    'ede' : 62/100000,
    'des' : 53/100000,
    'ela' : 45/100000,
    'ion' : 44/100000,
    'ait' : 44/100000,
    'res' : 44/100000,
    'tion' : 44/100000,
    'ment' : 31/100000,
    'ique' : 26/100000,
    'emen' : 25/100000,
    'dela' : 23/100000,
    'elle' : 21/100000,
    'dans' : 18/100000,
    'comme' : 1,
    'homme' : 1,
    'quand' : 1,
    'encore' : 1,
    'aussi' : 1,
    'temps' : 1,
    'petit' : 1,
    'avec' : 1,
    'tout' : 1,
    'main' : 1,
    'plus' : 1,
    'faire' : 1,
    'vous' : 1,
    'droit' : 1,
    'gauche' : 1,
    'presque' : 1,
    'chacun' : 1,
    'parfois' : 1,
    'depuis' : 1
}

# execute demo
if __name__ == "__main__":
    import sys

    key="hello"
    pt = "le repassage extreme est un melange de sport extreme et d'art de performance, dans lequel les adeptes installent une planche a repasser dans un lieu incongru, de preference dangereux, pour repasser quelques vetements. le site officiel le decrit comme le dernier sport a haut risque qui combine les frissons d'une activite extreme et la satisfaction d'une chemise bien repassee. le repassage extreme se pratique seul ou en groupe, il a deja ete pratique dans les lieux suivants : en haut d'une montagne apres l'avoir escaladee, dans une foret, en canoe, a ski ou en snowboard, au sommet d'une statue, au milieu d'une rue, sous l'eau ou encore sous la glace d'un lac gele.Un journal anglais a ecrit que cette activite s'inscrivait dans la traditionnelle excentricite britannique. l'interet des medias pour le repassage extreme tient plus a la question de savoir si c'est veritablement un sport qu'a l'activite elle-meme. les partisans de ce sport pretendent que tout a demarre en 1997 avec phil shaw, un habitant de leicester employe dans une usine de vetements en laine. alors qu'il rentrait d'une dure journee de labeur, il se retrouva devant sa pile de repassage. preferant passer la soiree a faire un peu d'escalade, il decida de combiner les deux activites et crea un nouveau sport extreme. en juin 1999, m.shaw, qui se fait surnommer steam (vapeur), debuta une tournee internationale pour promouvoir cette decouverte. ses principales etapes furent les etats-unis, les iles fidji, la nouvelle-zelande, l'australie et l'afrique du sud. une rencontre avec des touristes allemands en nouvelle-zelande provoqua la creation d'un groupe appele extreme ironing international et de son pendant germanique german extreme ironing section"
    ps = 1000
    birthRate = .40
    immigrationRate = .10
    
    print("usage: [echo message |] pygacb.py [key [population_size [birth_rate immigration_rate]]]")
    
    if len(sys.argv) >= 5:
        birthRate = float(sys.argv[3])    
        immigrationRate = float(sys.argv[4])    
    if len(sys.argv) >= 3:
        ps = int(sys.argv[2])    
    if len(sys.argv) >= 2:
        key = sys.argv[1]
    
    if sys.stdin.isatty() == False:
        pt = sys.stdin.read()
        
    charset = "azertyuiopqsdfghjklmwxcvbn0123456789"
    vigenere = Vigenere(charset)
    data = {'charset' : charset, 'vigenere' : vigenere, 'language' : french}
    ct = vigenere.encode(pt,key)
    gacb = GeneticAlgorithm(VigenereSolution, data, ct, ps, birthRate, immigrationRate)
    
    print("original text score: {}".format(gacb.best.simpleEval(pt)))
    print("real key: {}".format(key))
    print("population size: {}".format(ps))
    
    while (gacb.generation < 1000 and gacb.best.score < 0.5) or gacb.best.age < 30:
        if gacb.generation % 10 == 0:
            print("[generation {}]".format(gacb.generation))
        if gacb.best.age == 0:
            print("- {} ({})".format(gacb.best.key, gacb.best.score))
        gacb.evolve()