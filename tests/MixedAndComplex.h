#pragma once

class BaseComponent {
   public:
    int          id;
    virtual void process() = 0;
};

class Processor : public BaseComponent {
   public:
    void process() override;
    void configure();

   private:
    bool ready;
};

struct Settings {
    double version;
};

class ConfigurableProcessor : public Processor, public Settings {
   public:
    void loadSettings();
};

class Logger {
   public:
    static Logger& getInstance();

   private:
    Logger() {}
    Logger(const Logger&)            = delete;
    Logger& operator=(const Logger&) = delete;
};

class Event {};

class KeyEvent : public Event {};
class MouseEvent : public Event {};