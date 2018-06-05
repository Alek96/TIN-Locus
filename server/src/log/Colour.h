#ifndef LOCUS_COLOUR_H_INCLUDED
#define LOCUS_COLOUR_H_INCLUDED

#include <iostream>

namespace Log {

    struct Colour {
        enum Code {
            None = 0,

            White,
            Red,
            Green,
            Blue,
            Cyan,
            Yellow,
            Grey,

            Bright = 0x10,

            BrightRed = Bright | Red,
            BrightGreen = Bright | Green,
            LightGrey = Bright | Grey,
            BrightWhite = Bright | White,
            BrightYellow = Bright | Yellow,

            // By intention
                    FileName = LightGrey,
            Warning = BrightYellow,
            ResultError = BrightRed,
            ResultSuccess = BrightGreen,
            ResultExpectedFailure = Warning,

            Error = BrightRed,
            Success = Green,

            OriginalExpression = Cyan,
            ReconstructedExpression = BrightYellow,

            SecondaryText = LightGrey,
            Headers = White
        };

        // Use constructed object for RAII guard
        Colour(Code _colourCode);

        Colour(Colour &&other) noexcept;

        Colour &operator=(Colour &&other) noexcept;

        ~Colour();

        // Use static method for one-shot changes
        static void use(Code _colourCode);

    private:
        bool m_moved = false;
    };

    std::ostream &operator<<(std::ostream &os, Colour const &);

} // end Log namespace


#endif // LOCUS_COLOUR_H_INCLUDED
