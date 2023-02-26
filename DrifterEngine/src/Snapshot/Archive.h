#pragma once

namespace snapshot {

    class OutputArchive
    {
    public:
        template<typename... TArgs>
        void operator()(TArgs&&... args)
        {
            if (binary) {
                binary->operator()(std::forward<TArgs>(args)...);
            }
        }

        OutputArchive(cereal::BinaryOutputArchive& binary);

    private:
        cereal::BinaryOutputArchive* binary;
    };

    class InputArchive
    {
    public:
        template<typename... TArgs>
        void operator()(TArgs&&... args)
        {
            if (binary) {
                binary->operator()(std::forward<TArgs>(args)...);
            }
        }

        InputArchive(cereal::BinaryInputArchive& binary);

    private:
        cereal::BinaryInputArchive* binary;
    };

    class Archive
    {
    public:
        template<typename... TArgs>
        void operator()(TArgs&&... args)
        {
            if (binary_out) {
                binary_out->operator()(std::forward<TArgs>(args)...);
            }
            else {
                binary_in->operator()(std::forward<TArgs>(args)...);
            }
        }

        Archive(cereal::BinaryOutputArchive&);
        Archive(cereal::BinaryInputArchive&);

    private:
        void setNull();

    private:
        cereal::BinaryOutputArchive* binary_out;
        cereal::BinaryInputArchive* binary_in;
    };

} // namespace snapshot