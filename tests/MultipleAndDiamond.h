#pragma once

class Animal {
   public:
    void eat() {}
};

class Mammal : public Animal {
   public:
    void walk() {}
};

class Bird : public Animal {
   public:
    void fly() {}
};

class Bat : public Mammal, public Bird {
   public:
    void sonar() {}
};

class LivingBeing {};

class Creature : public LivingBeing {};
class Organism : public LivingBeing {};

class Entity : public Creature, public Organism {};