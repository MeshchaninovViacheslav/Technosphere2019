//
// Created by User on 04.04.2019.
//

#ifndef TECHNOSPERE_SERIALIZER_H
#define TECHNOSPERE_SERIALIZER_H
enum class Error {
    NoError,
    CorruptedArchive
};

int r = 0;

class Serializer {
    std::ostream &out_;
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream &out)
            : out_(out) {
    }

    template<class T>
    Error save(T &object) {
        return object.serialize(*this);
    }

    template<class... ArgsT>
    Error operator()(ArgsT&&... args) {
        return process(std::forward<ArgsT>(args)...);
    }

private:

    void print(uint64_t val) {
        out_ << val;
    }

    void print(bool val) {
        if (val)
            out_ << "true";
        else
            out_ << "false";
    }

    template<class T>
    Error process(T &&val) {
        print(val);
        out_ << '\n';
        return Error::NoError;
    }

    template<class T, class... ArgsT>
    Error process(T &&val, ArgsT &&... args) {
        print(val);
        out_ << Separator;
        process(std::forward<ArgsT>(args)...);
        return Error::NoError;
    }

};

class Deserializer {
    std::istream &in_;
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream &in)
            : in_(in) {
    }

    template<class T>
    Error load(T &object) {
        return object.serialize(*this);
    }

    template<class... ArgsT>
    Error operator()(ArgsT &&... args) {
        return process(args...);
    }

    template<class T, class... ArgsT>
    Error process(T &&val, ArgsT &&... args) {
        if (process(val) != Error::NoError)
            return Error::CorruptedArchive;
        return process(std::forward<ArgsT>(args)...);
    }

    Error process(bool &val) {
        std::string text;
        in_ >> text;

        if (text == "true")
            val = true;
        else if (text == "false")
            val = false;
        else
            return Error::CorruptedArchive;
        return Error::NoError;
    }

    Error process(uint64_t &val) {
        std::string text;
        in_ >> text;
        if (text.empty())
            return Error::CorruptedArchive;
        val = 0;
        for (auto i: text) {
            if (!(i >= '0' && i <= '9')) {
                return Error::CorruptedArchive;
            }
            val = val * 10 + i - '0';
        }
        return Error::NoError;
    }
};

#endif //TECHNOSPERE_SERIALIZER_H
