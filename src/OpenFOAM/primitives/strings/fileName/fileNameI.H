/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011 OpenFOAM Foundation
     \\/     M anipulation  | Copyright (C) 2016-2018 OpenCFD Ltd.
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include <iostream>  // for std::cerr

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

inline void Foam::fileName::stripInvalid()
{
    // Skip stripping unless debug is active (to avoid costly operations)
    if (debug && string::stripInvalid<fileName>(*this))
    {
        std::cerr
            << "fileName::stripInvalid() called for invalid fileName "
            << this->c_str() << std::endl;

        if (debug > 1)
        {
            std::cerr
                << "    For debug level (= " << debug
                << ") > 1 this is considered fatal" << std::endl;
            std::abort();
        }

        removeRepeated('/');
        removeTrailing('/');
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

inline Foam::fileName::fileName(const word& s)
:
    string(s)
{}


inline Foam::fileName::fileName(word&& s)
:
    string(std::move(s))
{}


inline Foam::fileName::fileName(const string& s, bool doStrip)
:
    string(s)
{
    if (doStrip)
    {
        stripInvalid();
    }
}


inline Foam::fileName::fileName(string&& s, bool doStrip)
:
    string(std::move(s))
{
    if (doStrip)
    {
        stripInvalid();
    }
}


inline Foam::fileName::fileName(const std::string& s, bool doStrip)
:
    string(s)
{
    if (doStrip)
    {
        stripInvalid();
    }
}


inline Foam::fileName::fileName(std::string&& s, bool doStrip)
:
    string(std::move(s))
{
    if (doStrip)
    {
        stripInvalid();
    }
}


inline Foam::fileName::fileName(const char* s, bool doStrip)
:
    string(s)
{
    if (doStrip)
    {
        stripInvalid();
    }
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

inline bool Foam::fileName::valid(char c)
{
    return
    (
        !isspace(c)
     && c != '"'   // string quote
     && c != '\''  // string quote
    );
}


inline bool Foam::fileName::isAbsolute(const std::string& str)
{
    return !str.empty() && str[0] == '/';
}


inline bool Foam::fileName::isAbsolute() const
{
    return isAbsolute(*this);
}


inline bool Foam::fileName::isBackup() const
{
    return isBackup(*this);
}


inline bool Foam::fileName::hasPath() const
{
    return string::hasPath();
}


inline bool Foam::fileName::hasExt() const
{
    return string::hasExt();
}


inline bool Foam::fileName::hasExt(const word& ending) const
{
    return string::hasExt(ending);
}


inline std::string Foam::fileName::path(const std::string& str)
{
    const auto i = str.rfind('/');

    if (i == npos)
    {
        return ".";
    }
    else if (i)
    {
        return str.substr(0, i);
    }

    return "/";
}


inline Foam::fileName Foam::fileName::path() const
{
    return path(*this);
}


inline std::string Foam::fileName::name(const std::string& str)
{
    const auto i = str.rfind('/');

    if (npos == i)
    {
        return str;
    }

    return str.substr(i+1);
}


inline Foam::word Foam::fileName::name() const
{
    return fileName::name(*this);
}


Foam::word Foam::fileName::ext() const
{
    return string::ext();
}


inline Foam::word Foam::fileName::nameLessExt() const
{
    return nameLessExt(*this);
}


inline Foam::fileName Foam::fileName::lessExt() const
{
    const auto i = find_ext();

    if (i == npos)
    {
        return *this;
    }

    return substr(0, i);
}


inline bool Foam::fileName::removePath()
{
    return string::removePath();
}


inline bool Foam::fileName::removeExt()
{
    return string::removeExt();
}


inline Foam::fileName& Foam::fileName::ext(const word& ending)
{
    string::ext(ending);
    return *this;
}


// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //

inline Foam::fileName& Foam::fileName::operator=(const word& str)
{
    assign(str);
    return *this;
}


inline Foam::fileName& Foam::fileName::operator=(word&& str)
{
    assign(std::move(str));
    return *this;
}


inline Foam::fileName& Foam::fileName::operator=(const string& str)
{
    assign(str);
    stripInvalid();
    return *this;
}


inline Foam::fileName& Foam::fileName::operator=(string&& str)
{
    assign(std::move(str));
    stripInvalid();
    return *this;
}


inline Foam::fileName& Foam::fileName::operator=(const std::string& str)
{
    assign(str);
    stripInvalid();
    return *this;
}


inline Foam::fileName& Foam::fileName::operator=(std::string&& str)
{
    assign(std::move(str));
    stripInvalid();
    return *this;
}


inline Foam::fileName& Foam::fileName::operator=(const char* str)
{
    assign(str);
    stripInvalid();
    return *this;
}


// ************************************************************************* //
