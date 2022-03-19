Vsak student ima na strezniku __lalgec.fri.uni-lj.si__ ustvarjen Subversion skladisce.

Primer podatkov za dostop:
- Elektronski naslov:       go3969@student.uni-lj.si
- Naslov skladisca:         https://lalgec.fri.uni-lj.si/os/ab1234
- Uporabnisko ime:          go3969
- Geslo:                    vase Active Directory Geslo (kot ga uporabljate za prijavo v racunalnike v racunalniskih ucilnicah)

Naslov skladisca kamor student oddaja svojo nalogo NalogaX je:
- https://lalgec.fri.uni-lj.si/os/go3969/NalogaX

1. Prenos Naloge iz skladisca
    - `svn checkout https://lalgec.fri.uni-lj.si/os/ab1234/Naloga0`
2. Shranjevanje vmesnih sprememb
    - `svn commit -m "Kratek opis sprememb."`
3. Oddajanje domace naloge (ko smo sigurni da dela)
    - `svn commit -m "#oddaj:Naloga0#"`
4. Osvezavanje delovne kpije in ogled rezultatov preverjanja (pocakat neki casa) 
    - `svn update .`
