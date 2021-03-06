/*---------------------------------------------------------------------------*\
modified from: OpenFOAM-3.0.0/src/thermophysicalModels/specie/transport/sutherland/sutherlandTransportI.H
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2012-2015 OpenFOAM Foundation
     \\/     M anipulation  |
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

#include "taitTg.H"
#include "specie.H"

// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

template<class Specie>
inline Foam::taitTg<Specie>::taitTg
(
    const Specie& sp,
    const scalar D2,
    const scalar D3,
    const scalar A1,
    const scalar A2,
    const scalar tauRlx,
    const scalar b1m,
    const scalar b2m,
    const scalar b3m,
    const scalar b4m,
    const scalar b1s,
    const scalar b2s,
    const scalar b3s,
    const scalar b4s,
    const scalar b5,
    const scalar b6,
    const scalar b10
)
:
    Specie(sp),
    D2_(D2),
    D3_(D3),
    A1_(A1),
    A2_(A2),
    tauRlx_(tauRlx),
    b1m_(b1m),
    b2m_(b2m),
    b3m_(b3m),
    b4m_(b4m),
    b1s_(b1s),
    b2s_(b2s),
    b3s_(b3s),
    b4s_(b4s),
    b5_(b5),
    b6_(b6),
    b10_(b10)
{}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Specie>
inline Foam::taitTg<Specie>::taitTg
(
    const word& name,
    const taitTg<Specie>& pf
)
:
    Specie(name, pf),
    D2_(pf.D2_),
    D3_(pf.D3_),
    A1_(pf.A1_),
    A2_(pf.A2_),
    tauRlx_(pf.tauRlx_),
    b1m_(pf.b1m_),
    b2m_(pf.b2m_),
    b3m_(pf.b3m_),
    b4m_(pf.b4m_),
    b1s_(pf.b1s_),
    b2s_(pf.b2s_),
    b3s_(pf.b3s_),
    b4s_(pf.b4s_),
    b5_(pf.b5_),
    b6_(pf.b6_),
    b10_(pf.b10_)
{}


template<class Specie>
inline Foam::autoPtr<Foam::taitTg<Specie> >
Foam::taitTg<Specie>::clone() const
{
    return autoPtr<taitTg<Specie> >(new taitTg<Specie>(*this));
}


template<class Specie>
inline Foam::autoPtr<Foam::taitTg<Specie> >
Foam::taitTg<Specie>::New(Istream& is)
{
    return autoPtr<taitTg<Specie> >(new taitTg<Specie>(is));
}


template<class Specie>
inline Foam::autoPtr<Foam::taitTg<Specie> >
Foam::taitTg<Specie>::New
(
    const dictionary& dict
)
{
    return autoPtr<taitTg<Specie> >(new taitTg<Specie>(dict));
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Specie>
inline Foam::scalar Foam::taitTg<Specie>::R() const
{
        Info << "POZOR: klic R() v taitTgI.H" << endl; // ce se to pjavi v izpisu simulacije bo treba preveriti, kaj so posledice te funkcije
    //return R_;
    return 1;
}


template<class Specie>
inline Foam::scalar Foam::taitTg<Specie>::rho(scalar p, scalar T) const // mathematica export fim_01_01_E_04_02
{
    scalar spec_volS;
    scalar spec_volM;
    scalar spec_vol;

    //melt
     spec_volM = (b1m_+b2m_*(-b5_+T))*(1-C_*log(1+p*pow(b3m_,-1)*exp(b4m_*(-b5_+T))));
    //solid
     spec_volS = (b1s_+b2s_*(-b5_+T))*(1-C_*log(1+p*pow(b3s_,-1)*exp(b4s_*(-b5_+T))));

     spec_vol = interpolation(
          spec_volS
         ,spec_volM
         ,p
         ,T
     );
    return 1.0/spec_vol;
}

template<class Specie>
inline Foam::scalar Foam::taitTg<Specie>::vg(scalar p, scalar T) const
{
    return (b1s_+b2s_*(-b5_+T))*(1-C_*log(1+p*pow(b3s_,-1)*exp(b4s_*(-b5_+T))));
}

template<class Specie>
inline Foam::scalar Foam::taitTg<Specie>::veq(scalar p, scalar T) const
{
    return scalar(1)/rho(p, T);
}

template<class Specie>
inline Foam::scalar Foam::taitTg<Specie>::vfeq(scalar p, scalar T) const
{

    return veq(p, T) - vg(p, T);
}

template<class Specie>
inline Foam::scalar Foam::taitTg<Specie>::tauRlx(scalar p, scalar T) const
{
    if(A2_+T-D2_ > VSMALL)
    {
        // limit exponent to 40 before exponentiation (returns ~1e17)
        return exp(
                min(40,
                    -(A1_*(T-D2_-D3_*p))/(A2_+T-D2_))
                ) * tauRlx_;
    }
    else
    {
        return VGREAT;
    }
}

template<class Specie>
inline Foam::scalar Foam::taitTg<Specie>::cTExp(scalar p, scalar T) const // num. der.
{
    scalar spec_volS;
    scalar spec_volSp;
    scalar spec_volSm;
    scalar spec_volM;
    scalar spec_volMp;
    scalar spec_volMm;
    scalar coefThermExp;

    scalar dT = 2.0; //[K]

    //melt
     spec_volMp = (b1m_+b2m_*(-b5_+T+dT))*(1-C_*log(1+p*pow(b3m_,-1)*exp(b4m_*(-b5_+T+dT))));
     spec_volMm = (b1m_+b2m_*(-b5_+T-dT))*(1-C_*log(1+p*pow(b3m_,-1)*exp(b4m_*(-b5_+T-dT))));
     spec_volM  = (b1m_+b2m_*(-b5_+T   ))*(1-C_*log(1+p*pow(b3m_,-1)*exp(b4m_*(-b5_+T   ))));
    //solid
     spec_volSp = (b1s_+b2s_*(-b5_+T+dT))*(1-C_*log(1+p*pow(b3s_,-1)*exp(b4s_*(-b5_+T+dT))));
     spec_volSm = (b1s_+b2s_*(-b5_+T-dT))*(1-C_*log(1+p*pow(b3s_,-1)*exp(b4s_*(-b5_+T-dT))));
     spec_volS  = (b1s_+b2s_*(-b5_+T   ))*(1-C_*log(1+p*pow(b3s_,-1)*exp(b4s_*(-b5_+T   ))));

     coefThermExp = interpolation( // this eliminates crystallization effect on thermal volume change
          (spec_volSp-spec_volSm)/(spec_volS*2.0*dT)
         ,(spec_volMp-spec_volMm)/(spec_volM*2.0*dT)
         ,p
         ,T
     );
    return coefThermExp;
}


template<class Specie>
inline Foam::scalar Foam::taitTg<Specie>::s(scalar p, scalar T) const // POZOR - NI TESTIRANO
{
    notImplemented("Foam::taitTg<Specie>::s");//kristjan

    return 0.0;
}

// A (b1m_+b2m_*(T-b5_))
// B (b3m_*exp(-b4m_*(T-b5_)))
 
template<class Specie>
inline Foam::scalar Foam::taitTg<Specie>::psi(scalar p, scalar T) const
{
    if (p < scalar(10.0)) //majhen tlak
    {
        Info << "Pozor! Majhen tlak, nevarnost deljenja z 0 v enacbi stanja, funkcija psi. p=" << p << " Pa" << endl;
    }

    return C_*exp(b4s_*(-b5_+T))*pow(b1s_+b2s_*(-b5_+T),-1)*pow(-1+C_*log(1+p*pow(b3s_,-1)*exp(b4s_*(-b5_+T))),-2)*pow(b3s_+p*exp(b4s_*(-b5_+T)),-1);
}


template<class Specie>
inline Foam::scalar Foam::taitTg<Specie>::Z(scalar p, scalar T) const
{
    Info << "POZOR: Z(p, T) v taitTgI.H" << endl; // ce se to pjavi v izpisu simulacije bo treba preveriti, kaj so posledice te funkcije
    return 1;
}


template<class Specie>
inline Foam::scalar Foam::taitTg<Specie>::cpMcv(scalar p, scalar T) const //fim_01_01_I_00_02_cpMcv_01 - MMA generated
{
    scalar cpmcvMy;
    scalar cpmcvMol;
    scalar Td = b10_; //[K] crystallization range
    scalar T0 = b5_ + b6_*p; //Tait solidification temperature
    scalar Tm = T0 - Td; //lower band limit

    if ( T > T0 )//melt
    {
        cpmcvMy = T*pow(C_,-1)*exp(-(b4m_*T))*(b3m_*exp(b4m_*b5_)+p*exp(b4m_*T))*pow(b1m_+b2m_*(-b5_+T),-1)*pow(-b2m_+b2m_*C_*log(1+p*pow(b3m_,-1)*exp(b4m_*(-b5_+T)))+b4m_*C_*p*(b1m_+b2m_*(-b5_+T))*exp(b4m_*T)*pow(b3m_*exp(b4m_*b5_)+p*exp(b4m_*T),-1),2);
    }
    else if ( T < Tm)//solid
    {
        cpmcvMy  = T*pow(C_,-1)*exp(-(b4s_*T))*(b3s_*exp(b4s_*b5_)+p*exp(b4s_*T))*pow(b1s_+b2s_*(-b5_+T),-1)*pow(-b2s_+b2s_*C_*log(1+p*pow(b3s_,-1)*exp(b4s_*(-b5_+T)))+b4s_*C_*p*(b1s_+b2s_*(-b5_+T))*exp(b4s_*T)*pow(b3s_*exp(b4s_*b5_)+p*exp(b4s_*T),-1),2);
    }
    else
    {
        cpmcvMy = -(T*pow(b10_,-3)*pow(-6*b6_*(b5_+b6_*p-T)*(-b10_+b5_+b6_*p-T)*(b1m_+b2m_*(-b5_+T))*(-1+C_*log(1+p*pow(b3m_,-1)*exp(b4m_*(-b5_+T))))+6*b6_*(b5_+b6_*p-T)*(-b10_+b5_+b6_*p-T)*(b1s_+b2s_*(-b5_+T))*(-1+C_*log(1+p*pow(b3s_,-1)*exp(b4s_*(-b5_+T))))-C_*(b10_+2*(b5_+b6_*p-T))*(b1m_+b2m_*(-b5_+T))*exp(b4m_*T)*pow(b10_-b5_-b6_*p+T,2)*pow(b3m_*exp(b4m_*b5_)+p*exp(b4m_*T),-1)+C_*(-3*b10_+2*(b5_+b6_*p-T))*(b1s_+b2s_*(-b5_+T))*exp(b4s_*T)*pow(b5_+b6_*p-T,2)*pow(b3s_*exp(b4s_*b5_)+p*exp(b4s_*T),-1),-1)*pow(6*(b5_+b6_*p-T)*(-b10_+b5_+b6_*p-T)*(b1m_+b2m_*(-b5_+T))*(-1+C_*log(1+p*pow(b3m_,-1)*exp(b4m_*(-b5_+T))))-6*(b5_+b6_*p-T)*(-b10_+b5_+b6_*p-T)*(b1s_+b2s_*(-b5_+T))*(-1+C_*log(1+p*pow(b3s_,-1)*exp(b4s_*(-b5_+T))))+b2s_*(-3*b10_+2*(b5_+b6_*p-T))*(-1+C_*log(1+p*pow(b3s_,-1)*exp(b4s_*(-b5_+T))))*pow(b5_+b6_*p-T,2)+b2m_*(b10_+2*(b5_+b6_*p-T))*(1-C_*log(1+p*pow(b3m_,-1)*exp(b4m_*(-b5_+T))))*pow(b10_-b5_-b6_*p+T,2)-b4m_*C_*p*(b10_+2*(b5_+b6_*p-T))*(b1m_+b2m_*(-b5_+T))*exp(b4m_*T)*pow(b10_-b5_-b6_*p+T,2)*pow(b3m_*exp(b4m_*b5_)+p*exp(b4m_*T),-1)+b4s_*C_*p*(-3*b10_+2*(b5_+b6_*p-T))*(b1s_+b2s_*(-b5_+T))*exp(b4s_*T)*pow(b5_+b6_*p-T,2)*pow(b3s_*exp(b4s_*b5_)+p*exp(b4s_*T),-1),2));
    }

     cpmcvMol = cpmcvMy  * this->W();
    //Info << "cpmcvM[J/(kmol K)]=" << cpmcvMol << " p=" << p << " T=" << T << endl;
    return cpmcvMol;
}

template<class Specie>
inline Foam::scalar Foam::taitTg<Specie>::interpolation(scalar vS ,scalar vM ,scalar p ,scalar T) const
{
    scalar Td = b10_; //[K] crystallization range
    scalar T0 = b5_ + b6_*p; //Tait solidification temperature
    scalar Tm = T0 - Td; //lower band limit

    if ( T > T0 )//melt
    {
        return vM;
    }
    else if ( T < Tm)
    {
        return vS;
    }
    else
    {
        scalar var = (T-T0)/b10_;
        scalar weig = 3.0*pow(var,2) + 2.0*pow(var,3);
        return (1.0 - weig) * vM + weig * vS;
    }

}
// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //
/*
template<class Specie>
inline void Foam::taitTg<Specie>::operator+=
(
    const taitTg<Specie>& pf
)
{
    scalar molr1 = this->nMoles();

    Specie::operator+=(pf);

    molr1 /= this->nMoles();
    scalar molr2 = pf.nMoles()/this->nMoles();

    R_ = 1.0/(molr1/R_ + molr2/pf.R_);
    rho0_ = molr1*rho0_ + molr2*pf.rho0_;
}


template<class Specie>
inline void Foam::taitTg<Specie>::operator-=
(
    const taitTg<Specie>& pf
)
{
    scalar molr1 = this->nMoles();

    Specie::operator-=(pf);

    molr1 /= this->nMoles();
    scalar molr2 = pf.nMoles()/this->nMoles();

    R_ = 1.0/(molr1/R_ - molr2/pf.R_);
    rho0_ = molr1*rho0_ - molr2*pf.rho0_;
}


template<class Specie>
inline void Foam::taitTg<Specie>::operator*=(const scalar s)
{
    Specie::operator*=(s);
}


// * * * * * * * * * * * * * * * Friend Operators  * * * * * * * * * * * * * //

template<class Specie>
inline Foam::taitTg<Specie> Foam::operator+
(
    const taitTg<Specie>& pf1,
    const taitTg<Specie>& pf2
)
{
    scalar nMoles = pf1.nMoles() + pf2.nMoles();
    scalar molr1 = pf1.nMoles()/nMoles;
    scalar molr2 = pf2.nMoles()/nMoles;

    return taitTg<Specie>
    (
        static_cast<const Specie&>(pf1)
      + static_cast<const Specie&>(pf2),
        1.0/(molr1/pf1.R_ + molr2/pf2.R_),
        molr1*pf1.rho0_ + molr2*pf2.rho0_
    );
}


template<class Specie>
inline Foam::taitTg<Specie> Foam::operator-
(
    const taitTg<Specie>& pf1,
    const taitTg<Specie>& pf2
)
{
    scalar nMoles = pf1.nMoles() + pf2.nMoles();
    scalar molr1 = pf1.nMoles()/nMoles;
    scalar molr2 = pf2.nMoles()/nMoles;

    return taitTg<Specie>
    (
        static_cast<const Specie&>(pf1)
      - static_cast<const Specie&>(pf2),
        1.0/(molr1/pf1.R_ - molr2/pf2.R_),
        molr1*pf1.rho0_ - molr2*pf2.rho0_
    );
}


template<class Specie>
inline Foam::taitTg<Specie> Foam::operator*
(
    const scalar s,
    const taitTg<Specie>& pf
)
{
    return taitTg<Specie>
    (
        s*static_cast<const Specie&>(pf),
        pf.R_,
        pf.rho0_
    );
}


template<class Specie>
inline Foam::taitTg<Specie> Foam::operator==
(
    const taitTg<Specie>& pf1,
    const taitTg<Specie>& pf2
)
{
    return pf2 - pf1;
}
*/

// ************************************************************************* //
