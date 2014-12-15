import random
import copy


class Molecule:
    def __init__(self, structure):
        self.pe = 0
        self.ke = 0
        self.opt = 9999999
        self.num_of_hits = 0
        self.structure = copy.deepcopy(structure)

    def update(self):
        """
        If this molecule has a lower energy, reset num_of_hits.
        """
        if self.pe < self.opt:
            self.opt = self.pe
            self.num_of_hits = 0


class CRO:
    """
    A base class represent the CRO algorithm.
    You should inherit from this class.
    """
    optimal = None  # A Molecule object represent the optimal solution
    cnt = 0
    limit = 1000
    KELossRate = 0.2
    mole_coll = 0.2
    alpha = 100
    beta = 100
    buffer = 0
    init_ke = 100
    pop = []

    def __init__(self, structure):
        """
        * fit_func: Object function
        * structure: Initial solution list [s1, s2, ..., sn]
        """
        for s in structure:
            self.pop.append(Molecule(s))

        for mol in self.pop:
            # You should implement this function in your derived class
            mol.pe = self.fit_func(mol)
            mol.ke = self.init_ke
            mol.update()
            if self.optimal is None:
                self.optimal = copy.deepcopy(mol)
            elif mol.pe < self.optimal.pe:
                self.optimal = copy.deepcopy(mol)

    def run(self):
        while self.cnt < self.limit:
            self.cnt += 1
            if random.random() > self.mole_coll or len(self.pop) < 2:
                m = random.choice(self.pop)
                if m.num_of_hits > self.alpha:
                    self.decomposition(m)
                else:
                    self.on_wall(m)
            else:
                m1, m2 = random.sample(self.pop, 2)
                if m1.ke <= self.beta and m2.ke <= self.beta:
                    self.synthesis(m1, m2)
                else:
                    self.interaction(m1, m2)

    def update(self, m):
        """
        If m is the current optimal solution, save it to the optimal.
        """
        if m.pe < self.optimal.pe:
            self.optimal = copy.deepcopy(m)

    def decomposition(self, m):
        m.num_of_hits += 1

        # You should implement this function in your derived class
        new1, new2 = self.dec(m)
        new1.pe = self.fit_func(new1)
        new2.pe = self.fit_func(new2)
        tmp = m.pe + m.ke - new1.pe - new2.pe
        if tmp >= 0 or tmp + self.buffer >= 0:
            if tmp >= 0:
                q = random.random()
                new1.ke = tmp * q
                new2.ke = tmp * (1 - q)
            else:
                new1.ke = (tmp + self.buffer) * random.random() * random.random()
                new2.ke = (tmp + self.buffer - new1.ke) * random.random() * random.random()
                self.buffer = self.buffer + tmp - new1.ke - new2.ke
            new1.update()
            new2.update()
            self.pop.remove(m)
            self.pop.append(new1)
            self.pop.append(new2)
            self.update(new1)
            self.update(new2)

    def on_wall(self, m):
        m.num_of_hits += 1

        # You should implement this function in your derived class
        new = self.wall(m)
        new.pe = self.fit_func(new)
        tmp = m.pe + m.ke - new.pe
        if tmp >= 0:
            q = random.uniform(self.KELossRate, 1)
            new.ke = tmp * q
            new.update()
            self.buffer += tmp * (1 - q)
            self.pop.remove(m)
            self.pop.append(new)
            self.update(new)

    def interaction(self, m1, m2):
        m1.num_of_hits += 1
        m2.num_of_hits += 1

        # You should implement this function in your derived class
        new1, new2 = self.inter(m1, m2)
        new1.pe = self.fit_func(new1)
        new2.pe = self.fit_func(new2)
        tmp = m1.pe + m2.pe + m1.ke + m2.ke - new1.pe - new2.pe
        if tmp >= 0:
            q = random.random()
            new1.ke = tmp * q
            new2.ke = tmp * (1 - q)
            new1.update()
            new2.update()
            self.pop.remove(m1)
            self.pop.remove(m2)
            self.pop.append(new1)
            self.pop.append(new2)
            self.update(new1)
            self.update(new2)

    def synthesis(self, m1, m2):
        m1.num_of_hits += 1
        m2.num_of_hits += 1

        # You should implement this function in your derived class
        new = self.syn(m1, m2)
        new.pe = self.fit_func(new)
        tmp = m1.pe + m2.pe + m1.ke + m2.ke - new.pe
        if tmp >= 0:
            new.ke = tmp
            new.update()
            self.pop.remove(m1)
            self.pop.remove(m2)
            self.pop.append(new)
            self.update(new)