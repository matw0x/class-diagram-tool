#pragma once

class Root {};

class Level1_1 : public Root {};
class Level1_2 : public Root {};
class Level1_3 : public Root {};
class Level1_4 : public Root {};

class Level2_1 : public Level1_1 {};
class Level2_2 : public Level1_1 {};
class Level2_3 : public Level1_2 {};
class Level2_4 : public Level1_2 {};
class Level2_5 : public Level1_3 {};
class Level2_6 : public Level1_3 {};
class Level2_7 : public Level1_4 {};
class Level2_8 : public Level1_4 {};

class Level3_1 : public Level2_1 {};
class Level3_2 : public Level2_3 {};
class Level3_3 : public Level2_5 {};
class Level3_4 : public Level2_7 {};