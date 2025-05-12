#pragma once

struct BaseContainer {};

struct DerivedContainer : public BaseContainer {};

class Item {};

struct ItemContainer : public BaseContainer, public Item {};