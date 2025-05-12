#pragma once

class Outer {
   public:
    class InnerClass {
        void innerMethod();
    };

    struct InnerStruct {
        int data;
    };

    // skip me
    void outerMethod();

    /*
    skip me again
    */
    class AnotherInnerClass : public InnerClass {};
};

class Standalone {};

struct Container {
    struct NestedData {
        double value;
    };
};