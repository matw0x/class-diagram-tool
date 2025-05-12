#pragma once

class Base {};

struct Intermediate1 : public Base {};

class Intermediate2 : public Base {};

struct FinalDerived : public Intermediate1, public Intermediate2 {};

class AnotherBase {};

struct SimpleStruct : public AnotherBase {};