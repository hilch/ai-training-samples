#include "Local.h"

/* ===== Cyclic entry point ===== */
void _CYCLIC MurksCYCLIC(void)
{
    murks_func001();
    murks_func002();
    murks_func003();
    murks_func004();
    murks_func005();
    murks_func006();
    murks_func007();
    murks_func008();
    murks_func009();
    murks_func010();
    murks_func011();
    murks_func012();
    murks_func013();
    murks_func014();
    murks_func015();
    murks_func016();
    murks_func017();
    murks_func018();
    murks_func019();
    murks_func020();
    murks_func021();
    murks_func022();
    murks_func023();
    murks_func024();
    murks_func025();
    murks_func026();
    murks_func027();
    murks_func028();
    murks_func029();
    murks_func030();
    murks_func031();
    murks_func032();
    murks_func033();
    murks_func034();
    murks_func035();
    murks_func036();
    murks_func037();
    murks_func038();
    murks_func039();
    murks_func040();
    murks_func041();
    murks_func042();
    murks_func043();
    murks_func044();
    murks_func045();
    murks_func046();
    murks_func047();
    murks_func048();
    murks_func049();
    murks_func050();
    murks_func051();
    murks_func052();
    murks_func053();
    murks_func054();
    murks_func055();
    murks_func056();
    murks_func057();
    murks_func058();
    murks_func059();
    murks_func060();
    murks_func061();
    murks_func062();
    murks_func063();
    murks_func064();
    murks_func065();
    murks_func066();
    murks_func067();
    murks_func068();
    murks_func069();
    murks_func070();
    murks_func071();
    murks_func072();
    murks_func073();
    murks_func074();
    murks_func075();
    murks_func076();
    murks_func077();
    murks_func078();
    murks_func079();
    murks_func080();
    murks_func081();
    murks_func082();
    murks_func083();
    murks_func084();
    murks_func085();
    murks_func086();
    murks_func087();
    murks_func088();
    murks_func089();
    murks_func090();
    murks_func091();
    murks_func092();
    murks_func093();
    murks_func094();
    murks_func095();
    murks_func096();
    murks_func097();
    murks_func098();
    murks_func099();
    murks_func100();
}

/* ===== 100 Dummy-Funktionen ===== */

/*
 * murks_func001 - Schleife mit bedingter Addition/Subtraktion;
 *   Switch auf Variablenwert, doppelt verschachtelte if-Pruefung.
 */
void murks_func001(void)
{
    SINT   a = 0;
    SINT   b = (SINT)cosf(0.0f);    /* math.h: cosf(0)=1 */
    SINT   c = 2;
    UINT   i = 0;
    for (i = 0; i < 10; i++)
    {
        if ((a + (b * i)) > (c - 1))
        {
            a = (SINT)(a + (SINT)i);
        }
        else
        {
            b = (SINT)(b - (SINT)(i * 2));
        }
    }
    switch (a)
    {
        case 0:  { c = 1; break; }
        case 1:  { c = 2; break; }
        case 2:  { c = 3; break; }
        default: { c = 0; break; }
    }
    for (i = 0; i < 5; i++)
    {
        if ((c > 0) && (b < 10))
        {
            if ((a >= 0) || (b != 0))
            {
                c = (SINT)(c + (SINT)i);
            }
        }
    }
}

/*
 * murks_func002 - Verschachtelte Schleifen mit Modulo-Pruefung
 *   und Ternary-Operator; Switch auf Ergebnis-Modulo.
 */
void murks_func002(void)
{
    UINT   x = 100;
    UINT   y = (UINT)fabsf(200.0f);    /* math.h: fabsf(200)=200 */
    UINT   z = 0;
    UINT   i = 0;
    UINT   j = 0;
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (((i + j) % 2) == 0)
            {
                z = (z + i) + j;
            }
            else
            {
                z = (z > 0) ? (z - 1) : 0;
            }
        }
    }
    switch (z % 4)
    {
        case 0:  { x = (y / 2);  break; }
        case 1:  { x = (y / 4);  break; }
        case 2:  { x = (y * 2);  break; }
        case 3:  { x = (y + 50); break; }
        default: { x = y;        break; }
    }
    if ((x > 0) && ((y - x) < 300))
    {
        z = x + y;
    }
}

/*
 * murks_func003 - Boolean-Flags mit Dreifach-Modulo-Schleife;
 *   Switch auf Zaehler, abschliessende Verdoppelungsbedingung.
 */
void murks_func003(void)
{
    BOOL   flag1 = 0;
    BOOL   flag2 = 1;
    BOOL   flag3 = 0;
    INT    counter = (INT)floorf(0.0f);    /* math.h: floorf(0)=0 */
    UINT   i = 0;
    for (i = 0; i < 20; i++)
    {
        if ((i % 3) == 0)
        {
            flag1 = 1;
            counter = counter + 1;
        }
        else if ((i % 3) == 1)
        {
            flag2 = 0;
        }
        else
        {
            flag3 = 1;
        }
    }
    switch (counter % 5)
    {
        case 0:  { flag1 = 0; flag2 = 1; break; }
        case 1:  { flag2 = 0; flag3 = 1; break; }
        case 2:  { flag3 = 0; flag1 = 1; break; }
        case 3:  { flag1 = 1; flag2 = 1; break; }
        case 4:  { flag3 = 1; flag2 = 0; break; }
        default: { break; }
    }
    if (((flag1 == 1) && (flag2 == 0)) || ((flag3 == 1) && (counter > 5)))
    {
        counter = counter * 2;
    }
}

/*
 * murks_func004 - Gleitkomma-Akkumulation in Schleife mit geradem/
 *   ungeradem Zweig; Switch auf getrunkierten Ergebniswert.
 */
void murks_func004(void)
{
    REAL   val1 = expf(0.0f);          /* math.h: expf(0)=1 */
    REAL   val2 = 2.5f;
    REAL   result = 0.0f;
    UINT   i = 0;
    for (i = 1; i <= 10; i++)
    {
        if ((i % 2) == 0)
        {
            result = result + (val1 * (REAL)i);
        }
        else
        {
            result = result + (val2 / (REAL)i);
        }
    }
    switch ((UINT)(result) % 3)
    {
        case 0:  { val1 = result / 2.0f;  break; }
        case 1:  { val2 = result * 0.5f;  break; }
        case 2:  { result = val1 + val2;  break; }
        default: { result = 0.0f;         break; }
    }
    if ((result > 0.0f) && (result < 100.0f))
    {
        if (((val1 + val2) > 3.0f) || (result > 10.0f))
        {
            result = result + 1.0f;
        }
    }
}

/*
 * murks_func005 - Bitweise Nibble-Extraktion und XOR-Verknuepfung
 *   auf UDINT; Switch auf niederwertiges Nibble des Ergebnisses.
 */
void murks_func005(void)
{
    UDINT  mask = 0x000000FFu;
    UDINT  data = 0xABCD1234u;
    UDINT  out  = (UDINT)sinf(0.0f);    /* math.h: sinf(0)=0 */
    UINT   i    = 0;
    for (i = 0; i < 4; i++)
    {
        UDINT shifted = (data >> (i * 8));
        UDINT nibble  = (shifted & mask);
        if (nibble > 0x80u)
        {
            out = (out | (nibble << (i * 2)));
        }
        else
        {
            out = (out ^ nibble);
        }
    }
    switch ((out & 0x0Fu))
    {
        case 0x00u: { data = 0u;    break; }
        case 0x01u: { data = out;   break; }
        case 0x02u: { data = ~out;  break; }
        case 0x03u: { data = out >> 1; break; }
        default:    { data = mask;  break; }
    }
    if (((data & 0xFF000000u) != 0u) && ((out & 0x00FF0000u) == 0u))
    {
        out = data & mask;
    }
}

/*
 * murks_func006 - Array-Initialisierung und Bubble-Sort;
 *   Summenberechnung und Switch auf Summen-Modulo.
 */
void murks_func006(void)
{
    INT    arr[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    INT    sum    = (INT)sqrtf(0.0f);    /* math.h: sqrtf(0)=0 */
    UINT   i      = 0;
    UINT   j      = 0;
    for (i = 0; i < 8; i++)
    {
        arr[i] = (INT)(i * 3) - 5;
    }
    for (i = 0; i < 7; i++)
    {
        for (j = i + 1; j < 8; j++)
        {
            if (arr[i] > arr[j])
            {
                INT tmp = arr[i];
                arr[i]  = arr[j];
                arr[j]  = tmp;
            }
        }
    }
    for (i = 0; i < 8; i++)
    {
        sum = sum + arr[i];
    }
    switch (sum % 6)
    {
        case 0:  { arr[0] = sum;     break; }
        case 1:  { arr[7] = sum / 2; break; }
        case 2:  { arr[3] = -sum;    break; }
        case 3:  { arr[4] = sum + 1; break; }
        case 4:  { arr[2] = sum - 1; break; }
        case 5:  { arr[5] = 0;       break; }
        default: { break; }
    }
}

/*
 * murks_func007 - Zustandsautomat mit 4 Zustaenden; Zustandsuebergaenge
 *   abhaengig von Schleifenindex und Zaehler.
 */
void murks_func007(void)
{
    UINT   state  = 0u;
    UINT   next   = 0u;
    UINT   count  = (UINT)tanf(0.0f);    /* math.h: tanf(0)=0 */
    UINT   i      = 0;
    for (i = 0; i < 15; i++)
    {
        switch (state)
        {
            case 0u:
            {
                if ((i % 2) == 0)
                {
                    next = 1u;
                }
                else
                {
                    next = 2u;
                }
                count = count + 1;
                break;
            }
            case 1u:
            {
                if ((i > 5) && (count < 10))
                {
                    next = 3u;
                }
                else
                {
                    next = 0u;
                }
                break;
            }
            case 2u:
            {
                next = (count > 7) ? 0u : 1u;
                break;
            }
            case 3u:
            {
                next = 0u;
                count = 0;
                break;
            }
            default:
            {
                next = 0u;
                break;
            }
        }
        state = next;
    }
}

/*
 * murks_func008 - Vorzeichenbehaftete Arithmetik mit positivem und
 *   negativem Zweig; Switch auf bedingtem Modulo-Wert.
 */
void murks_func008(void)
{
    DINT   a = -(DINT)fabsf(100.0f);    /* math.h: fabsf(100)=100, negated=-100 */
    DINT   b = 200;
    DINT   c = 0;
    UINT   i = 0;
    for (i = 0; i < 12; i++)
    {
        if ((a < 0) && (b > 0))
        {
            c = c + ((b - (-a)) / 2);
            a = a + 10;
        }
        else if ((a >= 0) && (b > 50))
        {
            c = c - 5;
            b = b - 20;
        }
        else
        {
            c = c + 1;
        }
    }
    switch ((c > 0) ? (c % 4) : 0)
    {
        case 0:  { a = c;      break; }
        case 1:  { b = c * 2;  break; }
        case 2:  { a = -c;     break; }
        case 3:  { b = c - 10; break; }
        default: { break; }
    }
    if (((a + b) > 0) && ((c * 2) < 1000))
    {
        c = a + b + c;
    }
}

/*
 * murks_func009 - Bit-fuer-Bit-XOR auf zwei Bytes; Nibble-Auswertung
 *   per Switch und abschliessende Masken-Operation.
 */
void murks_func009(void)
{
    USINT  nibble = (USINT)logf(1.0f);    /* math.h: logf(1)=0 */
    USINT  byte1  = 0xA5u;
    USINT  byte2  = 0x3Cu;
    UINT   i      = 0;
    for (i = 0; i < 8; i++)
    {
        nibble = (USINT)((byte1 >> i) & 0x01u);
        if (nibble != 0u)
        {
            byte2 = (USINT)(byte2 ^ (USINT)(1u << i));
        }
    }
    switch (byte2 & 0x0Fu)
    {
        case 0x00u: { byte1 = 0x00u; break; }
        case 0x05u: { byte1 = 0xFFu; break; }
        case 0x0Au: { byte1 = byte2; break; }
        case 0x0Fu: { byte1 = ~byte2; break; }
        default:    { byte1 = (USINT)(byte1 ^ byte2); break; }
    }
    for (i = 0; i < 4; i++)
    {
        if (((byte1 >> (i * 2)) & 0x03u) == 0x02u)
        {
            byte2 = (USINT)(byte2 | (USINT)(0x01u << i));
        }
    }
}

/*
 * murks_func010 - Fibonacci-aehnliche Folge mit Abbruchbedingung;
 *   Switch auf Folgen-Modulo und Mittelwert-Berechnung.
 */
void murks_func010(void)
{
    UINT   p = 1u;
    UINT   q = 1u;
    UINT   r = (UINT)ceilf(0.0f);    /* math.h: ceilf(0)=0 */
    UINT   i = 0;
    for (i = 2; i < 12; i++)
    {
        r = p + q;
        p = q;
        q = r;
        if (r > 100u)
        {
            break;
        }
    }
    switch (r % 7)
    {
        case 0u: { p = r / 7;  break; }
        case 1u: { p = r / 6;  break; }
        case 2u: { q = r / 5;  break; }
        case 3u: { q = r / 4;  break; }
        case 4u: { r = p + q;  break; }
        case 5u: { r = p * 2;  break; }
        case 6u: { r = q * 3;  break; }
        default: { break; }
    }
    for (i = 0; i < 5; i++)
    {
        if (((p + q) > r) || ((p * q) < 1000u))
        {
            r = (p + q + r) / 3;
        }
    }
}

/*
 * murks_func011 - 3x3-Matrix: Spur berechnen, Diagonale skalieren;
 *   Switch auf Spur-Modulo mit Matrix-Zuweisung.
 */
void murks_func011(void)
{
    INT    matrix[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    INT    trace = (INT)powf(0.0f, 1.0f);    /* math.h: powf(0,1)=0 */
    UINT   i     = 0;
    UINT   j     = 0;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (i == j)
            {
                trace = trace + matrix[i][j];
            }
            else if (i > j)
            {
                matrix[i][j] = matrix[i][j] * 2;
            }
            else
            {
                matrix[i][j] = matrix[i][j] - 1;
            }
        }
    }
    switch (trace % 5)
    {
        case 0:  { matrix[0][0] = trace;  break; }
        case 1:  { matrix[1][1] = trace;  break; }
        case 2:  { matrix[2][2] = trace;  break; }
        case 3:  { matrix[0][2] = -trace; break; }
        case 4:  { matrix[2][0] = trace * 2; break; }
        default: { break; }
    }
    if ((trace > 0) && (matrix[0][0] < 100))
    {
        trace = trace + matrix[2][2];
    }
}

/*
 * murks_func012 - 32-Bit-Manipulation: Bits setzen, loeschen, invertieren;
 *   Bit-Zaehlung und Switch auf Zaehlwert-Modulo.
 */
void murks_func012(void)
{
    UDINT  bits  = 0u;
    UDINT  mask  = 1u;
    UINT   count = (UINT)roundf(0.0f);    /* math.h: roundf(0)=0 */
    UINT   i     = 0;
    for (i = 0; i < 32; i++)
    {
        if ((i % 3) == 0)
        {
            bits = bits | (mask << i);
        }
        else if ((i % 3) == 1)
        {
            bits = bits & ~(mask << i);
        }
        else
        {
            bits = bits ^ (mask << i);
        }
    }
    for (i = 0; i < 32; i++)
    {
        if (((bits >> i) & 1u) != 0u)
        {
            count = count + 1;
        }
    }
    switch (count % 4)
    {
        case 0u: { bits = 0x00000000u; break; }
        case 1u: { bits = 0x0000FFFFu; break; }
        case 2u: { bits = 0xFFFF0000u; break; }
        case 3u: { bits = 0xFFFFFFFFu; break; }
        default: { break; }
    }
}

/*
 * murks_func013 - Gleitkomma-Pseudodreieck im Zustandsablauf;
 *   Switch auf Summen-Modulo und Mittelwert-Berechnung.
 */
void murks_func013(void)
{
    REAL   x = 0.0f;
    REAL   y = 1.0f;
    REAL   z = sqrtf(4.0f);    /* math.h: sqrtf(4)=2 */
    UINT   i = 0;
    for (i = 0; i < 10; i++)
    {
        REAL tmp = (x * x) + (y * y);
        if (tmp < 100.0f)
        {
            x = x + (y * 0.1f);
            y = y + (z * 0.05f);
        }
        else if (tmp < 200.0f)
        {
            x = x - 0.5f;
            z = z + 0.2f;
        }
        else
        {
            x = 0.0f;
            y = 1.0f;
        }
    }
    switch ((UINT)(x + y + z) % 3)
    {
        case 0u: { z = x + y; break; }
        case 1u: { z = x * y; break; }
        case 2u: { z = x - y; break; }
        default: { break; }
    }
    if ((z > 0.0f) && ((x + y) < 50.0f))
    {
        z = (x + y + z) / 3.0f;
    }
}

/*
 * murks_func014 - Min, Max und Summe ueber fuenf Elemente;
 *   Switch auf Differenz-Modulo mit Array-Zuweisung.
 */
void murks_func014(void)
{
    UINT   a[5] = {5u, 3u, 8u, 1u, 9u};
    UINT   min  = a[0];
    UINT   max  = a[0];
    UINT   sum  = (UINT)fabsf(0.0f);    /* math.h: fabsf(0)=0 */
    UINT   i    = 0;
    for (i = 0; i < 5; i++)
    {
        if (a[i] < min)
        {
            min = a[i];
        }
        if (a[i] > max)
        {
            max = a[i];
        }
        sum = sum + a[i];
    }
    switch ((max - min) % 5)
    {
        case 0u: { a[0] = min;      break; }
        case 1u: { a[4] = max;      break; }
        case 2u: { a[2] = sum / 5u; break; }
        case 3u: { a[1] = max - min; break; }
        case 4u: { a[3] = sum;      break; }
        default: { break; }
    }
    for (i = 0; i < 5; i++)
    {
        if ((a[i] > 5u) && (a[i] < 20u))
        {
            sum = sum - a[i];
        }
    }
}

/*
 * murks_func015 - Vorzeichenbehaftete LUT-Akkumulation mit
 *   Spiegelindex; Switch auf Akkumulator-Modulo.
 */
void murks_func015(void)
{
    SINT   lut[8] = {10, -5, 3, -8, 15, 0, -2, 7};
    INT    acc    = (INT)logf(1.0f);    /* math.h: logf(1)=0 */
    UINT   idx    = 0u;
    UINT   i      = 0;
    for (i = 0; i < 16; i++)
    {
        idx = (UINT)(i % 8);
        if ((i % 2) == 0)
        {
            acc = acc + (INT)lut[idx];
        }
        else
        {
            acc = acc - (INT)lut[(7u - idx)];
        }
    }
    switch (acc % 7)
    {
        case 0:  { lut[0] = (SINT)(acc & 0x7F); break; }
        case 1:  { lut[1] = (SINT)(-acc & 0x7F); break; }
        case 2:  { lut[3] = (SINT)(acc / 2); break; }
        case 3:  { lut[5] = (SINT)(acc % 10); break; }
        case 4:  { lut[7] = (SINT)(acc - 5); break; }
        case 5:  { lut[2] = 0; break; }
        case 6:  { lut[4] = (SINT)(acc + 1); break; }
        default: { break; }
    }
    if (((acc > -50) && (acc < 50)) || (idx > 4u))
    {
        acc = (INT)lut[0] + (INT)lut[7];
    }
}

/*
 * murks_func016 - Duty-Cycle-Steuerung mit Phasenzaehler;
 *   Switch auf Duty-Modulo und Phasenberechnung.
 */
void murks_func016(void)
{
    UINT   duty    = (UINT)fabsf(50.0f);    /* math.h: fabsf(50)=50 */
    UINT   period  = 100u;
    UINT   phase   = 0u;
    UINT   i       = 0;
    for (i = 0; i < period; i++)
    {
        phase = i % period;
        if (phase < duty)
        {
            if (phase < (duty / 2u))
            {
                duty = duty + 1u;
            }
        }
        else
        {
            if (phase > ((period + duty) / 2u))
            {
                duty = (duty > 0u) ? (duty - 1u) : 0u;
            }
        }
    }
    switch (duty % 10u)
    {
        case 0u: { period = 100u; break; }
        case 5u: { period = 50u;  break; }
        default: { period = 200u; break; }
    }
    if ((duty > 0u) && (duty < period))
    {
        phase = duty * period / 100u;
    }
}

/*
 * murks_func017 - Stack-Simulation mit Push/Pop; arithmetische
 *   Operationen (Add, Sub, Div) per Switch-gesteuertem Pop.
 */
void murks_func017(void)
{
    INT    stack[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    UINT   top       = 0u;
    INT    val       = (INT)sinf(0.0f);    /* math.h: sinf(0)=0 */
    UINT   i         = 0;
    for (i = 0; i < 8; i++)
    {
        val = (INT)((i * 7) % 11) - 3;
        if (top < 10u)
        {
            stack[top] = val;
            top = top + 1u;
        }
    }
    for (i = 0; i < 3; i++)
    {
        if (top > 1u)
        {
            INT b = stack[top - 1u]; top--;
            INT a = stack[top - 1u]; top--;
            switch (i % 3)
            {
                case 0:  { stack[top] = a + b; top++; break; }
                case 1:  { stack[top] = a - b; top++; break; }
                case 2:  { stack[top] = (b != 0) ? (a / b) : 0; top++; break; }
                default: { break; }
            }
        }
    }
    if ((top > 0u) && (stack[0] != 0))
    {
        val = stack[0] * 2;
    }
}

/*
 * murks_func018 - CRC32-Berechnung (MSB-first) ueber 4 Datenbytes;
 *   Switch auf oberem Nibble des CRC-Ergebnisses.
 */
void murks_func018(void)
{
    UDINT  crc   = 0xFFFFFFFFu;
    UDINT  poly  = 0x04C11DB7u;
    UINT   i     = 0;
    UINT   j     = (UINT)tanf(0.0f);    /* math.h: tanf(0)=0 */
    USINT  data[4] = {0xDEu, 0xADu, 0xBEu, 0xEFu};
    for (i = 0; i < 4; i++)
    {
        crc = crc ^ ((UDINT)data[i] << 24u);
        for (j = 0; j < 8; j++)
        {
            if ((crc & 0x80000000u) != 0u)
            {
                crc = ((crc << 1u) ^ poly);
            }
            else
            {
                crc = (crc << 1u);
            }
        }
    }
    crc = crc ^ 0xFFFFFFFFu;
    switch ((crc >> 28u) & 0x0Fu)
    {
        case 0u:  { poly = 0u;   break; }
        case 0xFu: { poly = crc; break; }
        default:  { poly = (crc ^ 0xA5A5A5A5u); break; }
    }
    if ((crc != 0u) && (poly != 0xFFFFFFFFu))
    {
        data[0] = (USINT)(crc & 0xFFu);
    }
}

/*
 * murks_func019 - Teilbarkeits-Zaehlung bis 1000 mit zwei
 *   Modulo-Bedingungen; Switch auf Zaehler-Modulo.
 */
void murks_func019(void)
{
    UINT   r = 37u;
    UINT   n = 1000u;
    UINT   i = 0;
    UINT   cnt = (UINT)floorf(0.0f);    /* math.h: floorf(0)=0 */
    for (i = 1; i <= n; i++)
    {
        if ((i % r) == 0u)
        {
            cnt = cnt + 1u;
        }
        else if ((i % (r / 2u + 1u)) == 0u)
        {
            cnt = cnt + 2u;
        }
    }
    switch (cnt % 8u)
    {
        case 0u: { r = 37u;  break; }
        case 1u: { r = 41u;  break; }
        case 2u: { r = 43u;  break; }
        case 3u: { r = 47u;  break; }
        case 4u: { r = 53u;  break; }
        case 5u: { r = 59u;  break; }
        case 6u: { r = 61u;  break; }
        case 7u: { r = 67u;  break; }
        default: { break; }
    }
    if ((cnt > 10u) && ((n / r) < 50u))
    {
        cnt = cnt / 2u;
    }
}

/*
 * murks_func020 - Dreiecks-Signalerzeugung in Puffer mit
 *   Amplitudensteuerung; Switch auf Amplitude-Modulo.
 */
void murks_func020(void)
{
    REAL   angle = 0.0f;
    REAL   step  = 0.314159f;
    REAL   amp   = expf(0.0f);    /* math.h: expf(0)=1 */
    UINT   i     = 0;
    REAL   buf[5] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    for (i = 0; i < 5; i++)
    {
        angle = step * (REAL)i;
        if ((angle > 0.0f) && (angle < 1.0f))
        {
            buf[i] = amp * angle;
        }
        else if ((angle >= 1.0f) && (angle < 2.0f))
        {
            buf[i] = amp * (2.0f - angle);
        }
        else
        {
            buf[i] = 0.0f;
        }
    }
    switch ((UINT)(amp * 10.0f) % 3u)
    {
        case 0u: { amp = 1.0f;  break; }
        case 1u: { amp = 0.5f;  break; }
        case 2u: { amp = 2.0f;  break; }
        default: { break; }
    }
    if ((buf[2] > 0.0f) && (amp > 0.0f))
    {
        buf[4] = buf[0] + buf[1] + buf[2] + buf[3];
    }
}

/*
 * murks_func021 - Euklidischer Algorithmus (GGT) mit anschliessender
 *   Nachbearbeitung in Schleife; Switch auf GGT-Modulo.
 */
void murks_func021(void)
{
    UINT   a = 9u;
    UINT   b = 6u;
    UINT   gcd = (UINT)sinf(0.0f);    /* math.h: sinf(0)=0 */
    UINT   tmp = 0u;
    UINT   i   = 0;
    while ((a != 0u) && (b != 0u))
    {
        if (a > b)
        {
            a = a % b;
        }
        else
        {
            b = b % a;
        }
    }
    gcd = (a == 0u) ? b : a;
    for (i = 0; i < 10; i++)
    {
        tmp = (UINT)((gcd * i) % 7u);
        if (tmp == 0u)
        {
            gcd = gcd + 1u;
        }
        else
        {
            gcd = (gcd > 1u) ? (gcd - 1u) : 1u;
        }
    }
    switch (gcd % 6u)
    {
        case 0u: { a = gcd;       break; }
        case 1u: { b = gcd * 2u;  break; }
        case 2u: { a = b + gcd;   break; }
        case 3u: { b = a + gcd;   break; }
        case 4u: { gcd = a + b;   break; }
        case 5u: { gcd = a * b;   break; }
        default: { break; }
    }
}

/*
 * murks_func022 - Temperatur/Feuchte/Druck-Simulation ueber
 *   20 Zyklen; Switch auf Druckwert-Modulo.
 */
void murks_func022(void)
{
    SINT   temp   = (SINT)logf(1.0f);    /* math.h: logf(1)=0 */
    SINT   humid  = 0;
    SINT   press  = 0;
    UINT   i      = 0;
    for (i = 0; i < 20; i++)
    {
        temp   = (SINT)((i * 3) % 50);
        humid  = (SINT)((i * 7) % 100);
        press  = (SINT)((i * 13) % 120);
        if ((temp > 20) && (humid > 60))
        {
            press = (SINT)(press + 5);
        }
        else if ((temp < 10) && (humid < 30))
        {
            press = (SINT)(press - 3);
        }
        switch (press % 4)
        {
            case 0:  { temp = (SINT)(temp + 1);  break; }
            case 1:  { humid = (SINT)(humid - 1); break; }
            case 2:  { press = (SINT)(press / 2); break; }
            case 3:  { temp = (SINT)(-temp);      break; }
            default: { break; }
        }
    }
}

/*
 * murks_func023 - RGB-Pixel-Berechnung mit Helligkeitsinversion;
 *   Switch auf niederwertigen Bits des Pixels.
 */
void murks_func023(void)
{
    UINT   pixel  = (UINT)tanf(0.0f);    /* math.h: tanf(0)=0 */
    UINT   red    = 0u;
    UINT   green  = 0u;
    UINT   blue   = 0u;
    UINT   i      = 0;
    for (i = 0; i < 256; i += 16)
    {
        red   = (i & 0xE0u) >> 5u;
        green = (i & 0x1Cu) >> 2u;
        blue  = (i & 0x03u);
        if (red > 4u)
        {
            red = 7u - red;
        }
        if (green > 3u)
        {
            green = green ^ 0x07u;
        }
        pixel = (red << 5u) | (green << 2u) | blue;
    }
    switch (pixel & 0x03u)
    {
        case 0u: { blue  = 0u; break; }
        case 1u: { blue  = 1u; break; }
        case 2u: { blue  = 2u; break; }
        case 3u: { blue  = 3u; break; }
        default: { break; }
    }
    if ((red + green + blue) < 20u)
    {
        pixel = (pixel + 1u) & 0xFFu;
    }
}

/*
 * murks_func024 - Ereignisgesteuerter Zustandsautomat mit 5 Ereignissen
 *   und 4 Zustaenden; Ausgabe-Akkumulation im letzten Zustand.
 */
void murks_func024(void)
{
    UINT   fsm   = 0u;
    UINT   ev    = 0u;
    UINT   out   = (UINT)ceilf(0.0f);    /* math.h: ceilf(0)=0 */
    UINT   i     = 0;
    for (i = 0; i < 20; i++)
    {
        ev = (UINT)(i % 5u);
        switch (fsm)
        {
            case 0u:
            {
                if (ev == 1u) { fsm = 1u; out = 10u; }
                else if (ev == 2u) { fsm = 2u; out = 20u; }
                else { out = 0u; }
                break;
            }
            case 1u:
            {
                if (ev == 3u) { fsm = 3u; out = 30u; }
                else if (ev == 0u) { fsm = 0u; out = 0u; }
                break;
            }
            case 2u:
            {
                if ((ev == 4u) || (ev == 1u))
                {
                    fsm = 0u;
                    out = 5u;
                }
                break;
            }
            case 3u:
            {
                fsm = 0u;
                out = out + i;
                break;
            }
            default:
            {
                fsm = 0u;
                out = 0u;
                break;
            }
        }
    }
}

/*
 * murks_func025 - djb2-Hash ueber den String "Murks" mit
 *   XOR-Scrambling und Null-Bit-Auffuellung.
 */
void murks_func025(void)
{
    UDINT  hash  = (UDINT)fabsf(5381.0f);    /* math.h: fabsf(5381)=5381 */
    USINT  str[6] = {0x4Du, 0x75u, 0x72u, 0x6Bu, 0x73u, 0x00u};
    UINT   i     = 0;
    while ((i < 6u) && (str[i] != 0x00u))
    {
        hash = ((hash << 5u) + hash) + (UDINT)str[i];
        i    = i + 1u;
    }
    switch (hash % 8u)
    {
        case 0u: { hash ^= 0xDEADBEEFu; break; }
        case 1u: { hash ^= 0xCAFEBABEu; break; }
        case 2u: { hash ^= 0xFEEDFACEu; break; }
        case 3u: { hash ^= 0xBAADF00Du; break; }
        case 4u: { hash ^= 0x0D15EA5Eu; break; }
        case 5u: { hash ^= 0xABABABABu; break; }
        case 6u: { hash ^= 0x12345678u; break; }
        case 7u: { hash ^= 0x87654321u; break; }
        default: { break; }
    }
    for (i = 0; i < 4u; i++)
    {
        if (((hash >> (i * 8u)) & 0xFFu) == 0u)
        {
            hash = hash | (0x01u << (i * 8u));
        }
    }
}

/*
 * murks_func026 - Ganzzahl-Division mit Rest und iterativer
 *   Neuberechnung; Switch auf Rest-Modulo.
 */
void murks_func026(void)
{
    INT    q = (INT)sqrtf(0.0f);    /* math.h: sqrtf(0)=0 */
    INT    r = 0;
    INT    n = 255;
    INT    d = 7;
    UINT   i = 0;
    if (d != 0)
    {
        q = n / d;
        r = n % d;
    }
    for (i = 0; i < 10; i++)
    {
        if ((q > 0) && (r >= 0))
        {
            q = q - 1;
            r = r + d;
        }
        else if (q < 0)
        {
            q = 0;
            r = n;
        }
    }
    switch (r % 4)
    {
        case 0:  { n = q * d + r; break; }
        case 1:  { n = q + r;     break; }
        case 2:  { n = q - r;     break; }
        case 3:  { n = q * r;     break; }
        default: { n = 0;         break; }
    }
    if ((n > -1000) && (n < 1000))
    {
        q = n / (d != 0 ? d : 1);
        r = n % (d != 0 ? d : 1);
    }
}

/*
 * murks_func027 - 8-Bit-Addierer Bit-fuer-Bit mit Carry-Propagation;
 *   Switch auf XOR-Ergebnis der Ausgaenge.
 */
void murks_func027(void)
{
    UINT   in1  = 0xABu;
    UINT   in2  = 0xCDu;
    UINT   out1 = (UINT)powf(0.0f, 1.0f);    /* math.h: powf(0,1)=0 */
    UINT   out2 = 0u;
    UINT   i    = 0;
    for (i = 0; i < 8; i++)
    {
        UINT b1 = (in1 >> i) & 1u;
        UINT b2 = (in2 >> i) & 1u;
        UINT carry = (i > 0) ? ((out1 >> (i - 1)) & 1u) : 0u;
        UINT sum   = b1 + b2 + carry;
        if ((sum & 1u) != 0u)
        {
            out1 = out1 | (1u << i);
        }
        if (sum > 1u)
        {
            out2 = out2 | (1u << i);
        }
    }
    switch ((out1 ^ out2) & 0x0Fu)
    {
        case 0u:  { in1 = 0u;  break; }
        case 0xFu: { in2 = 0u; break; }
        default:  { in1 ^= in2; break; }
    }
    if ((out1 != 0u) && (out2 != 0xFFu))
    {
        out1 = (out1 + out2) & 0xFFu;
    }
}

/*
 * murks_func028 - PID-Regler mit Anti-Windup und Output-Begrenzung;
 *   Switch auf Integral-Modulo mit Koeffizienten-Anpassung.
 */
void murks_func028(void)
{
    REAL   pid_p = logf(1.0f);    /* math.h: logf(1)=0 */
    REAL   pid_i = 0.0f;
    REAL   pid_d = 0.0f;
    REAL   err   = 5.0f;
    REAL   prev  = 0.0f;
    REAL   kp    = 0.8f;
    REAL   ki    = 0.1f;
    REAL   kd    = 0.05f;
    UINT   i     = 0;
    for (i = 0; i < 10; i++)
    {
        pid_p = kp * err;
        pid_i = pid_i + (ki * err);
        pid_d = kd * (err - prev);
        prev  = err;
        REAL output = pid_p + pid_i + pid_d;
        if (output > 10.0f)
        {
            output = 10.0f;
        }
        else if (output < -10.0f)
        {
            output = -10.0f;
        }
        err = err - (output * 0.1f);
        if ((err > -0.01f) && (err < 0.01f))
        {
            break;
        }
    }
    switch ((UINT)(pid_i * 10.0f) % 3u)
    {
        case 0u: { pid_i = 0.0f;  break; }
        case 1u: { kp *= 1.1f;    break; }
        case 2u: { ki *= 0.9f;    break; }
        default: { break; }
    }
}

/*
 * murks_func029 - Ganzzahlige Quadratwurzel durch Iteration;
 *   Switch auf Wurzel-Modulo mit Quadrat-Berechnung.
 */
void murks_func029(void)
{
    UINT   n    = 64u;
    UINT   root = (UINT)cosf(0.0f);    /* math.h: cosf(0)=1 */
    UINT   i    = 0;
    while (((root + 1u) * (root + 1u)) <= n)
    {
        root = root + 1u;
    }
    for (i = 0; i < root; i++)
    {
        if ((i * i) < n)
        {
            if (((i + 1u) * (i + 1u)) > n)
            {
                root = i;
                break;
            }
        }
    }
    switch (root % 4u)
    {
        case 0u: { n = root * root;            break; }
        case 1u: { n = (root + 1u) * (root + 1u); break; }
        case 2u: { n = root * (root + 2u);     break; }
        case 3u: { n = (root - 1u) * (root + 1u); break; }
        default: { n = 0u; break; }
    }
    if ((root > 0u) && (n > 0u))
    {
        root = (root + n) / 2u;
    }
}

/*
 * murks_func030 - Pythagoras: Hypotenuse durch Iteration;
 *   Switch auf Hypotenuse-Modulo und Seiten-Vertauschung.
 */
void murks_func030(void)
{
    SINT   a = (SINT)fabsf(5.0f);    /* math.h: fabsf(5)=5 */
    SINT   b = 12;
    SINT   hyp = 0;
    SINT   sq  = 0;
    UINT   i   = 0;
    sq = (a * a) + (b * b);
    for (i = 1; i <= 20; i++)
    {
        if (((SINT)(i * i)) <= sq)
        {
            hyp = (SINT)i;
        }
        else
        {
            break;
        }
    }
    switch (hyp % 5)
    {
        case 0:  { a = hyp;          break; }
        case 1:  { b = hyp;          break; }
        case 2:  { a = (hyp / 2);    break; }
        case 3:  { b = (hyp - a);    break; }
        case 4:  { a = b; b = hyp;   break; }
        default: { break; }
    }
    for (i = 0; i < 5; i++)
    {
        if (((a + b) > hyp) && (((a * b) % 3) == 0))
        {
            hyp = (SINT)(a + b);
        }
    }
}

/*
 * murks_func031 - Dreiecks-Signalgenerator mit Frequenz und Amplitude;
 *   Switch auf Sample-Modulo mit Frequenz-/Amplitudenanpassung.
 */
void murks_func031(void)
{
    UINT   freq  = (UINT)fabsf(1000.0f);    /* math.h: fabsf(1000)=1000 */
    UINT   amp2  = 100u;
    UINT   phase2 = 0u;
    UINT   i     = 0;
    UINT   samp  = 0u;
    for (i = 0; i < 100; i++)
    {
        phase2 = (phase2 + freq) % 360000u;
        samp   = (UINT)(amp2 * (phase2 / 180000u));
        if (samp > amp2)
        {
            samp = (2u * amp2) - samp;
        }
        if ((samp < 10u) && (i > 50u))
        {
            freq = freq + 10u;
        }
        else if ((samp > 90u) && (i < 50u))
        {
            freq = (freq > 10u) ? (freq - 10u) : 10u;
        }
    }
    switch (samp % 5u)
    {
        case 0u: { amp2  = 50u;  break; }
        case 1u: { amp2  = 75u;  break; }
        case 2u: { amp2  = 100u; break; }
        case 3u: { freq  = 500u; break; }
        case 4u: { freq  = 2000u; break; }
        default: { break; }
    }
}

/*
 * murks_func032 - Einfacher IIR-Tiefpass/-Hochpassfilter;
 *   Switch auf Schleifenindex-Modulo mit Signalauswahl.
 */
void murks_func032(void)
{
    DINT   lp = (DINT)floorf(0.0f);    /* math.h: floorf(0)=0 */
    DINT   hp = 0;
    DINT   x  = 1000;
    REAL   fc = 0.1f;
    UINT   i  = 0;
    for (i = 0; i < 20; i++)
    {
        DINT input = (DINT)((i % 5) * 200) - 400;
        lp = lp + (DINT)(fc * (REAL)(input - lp));
        hp = input - lp;
        if ((lp > 800) || (lp < -800))
        {
            lp = (lp > 0) ? 800 : -800;
        }
        switch ((UINT)(i) % 4u)
        {
            case 0u: { x = lp;       break; }
            case 1u: { x = hp;       break; }
            case 2u: { x = lp + hp;  break; }
            case 3u: { x = lp - hp;  break; }
            default: { x = 0;        break; }
        }
    }
    if ((x > -2000) && (x < 2000))
    {
        lp = (lp + x) / 2;
    }
}

/*
 * murks_func033 - Ringpuffer mit Schreib-/Lesezeiger; Groessen-
 *   Management und Switch auf Fuellstand-Modulo.
 */
void murks_func033(void)
{
    UINT   ring[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
    UINT   head  = (UINT)sqrtf(0.0f);    /* math.h: sqrtf(0)=0 */
    UINT   tail  = 0u;
    UINT   size  = 0u;
    UINT   i     = 0;
    for (i = 0; i < 15; i++)
    {
        if (size < 8u)
        {
            ring[head] = (UINT)(i * 3u + 1u);
            head = (head + 1u) % 8u;
            size = size + 1u;
        }
        if ((i % 3u == 0u) && (size > 0u))
        {
            UINT val = ring[tail];
            tail = (tail + 1u) % 8u;
            size = size - 1u;
            if (val > 20u)
            {
                head = (head + 1u) % 8u;
            }
        }
    }
    switch (size % 4u)
    {
        case 0u: { head = 0u; tail = 0u; break; }
        case 1u: { head = (head + 1u) % 8u; break; }
        case 2u: { tail = (tail + 1u) % 8u; break; }
        case 3u: { size = 0u; break; }
        default: { break; }
    }
}

/*
 * murks_func034 - Bitweise Logik-Gatter (AND, OR, XOR, MUX) Bit-fuer-Bit;
 *   Switch auf Ergebnis-Modulo mit Eingangs-Anpassung.
 */
void murks_func034(void)
{
    UINT   en1  = (UINT)cosf(0.0f);    /* math.h: cosf(0)=1 */
    UINT   en2  = 0u;
    UINT   en3  = 1u;
    UINT   out3 = 0u;
    UINT   i    = 0;
    for (i = 0; i < 8; i++)
    {
        UINT a = (en1 >> i) & 1u;
        UINT b = (en2 >> i) & 1u;
        UINT c = (en3 >> i) & 1u;
        UINT and_ab = a & b;
        UINT or_ac  = a | c;
        UINT xor_bc = b ^ c;
        UINT mux    = (a != 0u) ? or_ac : and_ab;
        if (((and_ab | xor_bc) & (or_ac ^ mux)) != 0u)
        {
            out3 = out3 | (1u << i);
        }
    }
    switch (out3 % 6u)
    {
        case 0u: { en1 = 0xFFu; break; }
        case 1u: { en2 = 0xFFu; break; }
        case 2u: { en3 = 0x00u; break; }
        case 3u: { en1 ^= en2;  break; }
        case 4u: { en2 ^= en3;  break; }
        case 5u: { en3 ^= en1;  break; }
        default: { break; }
    }
}

/*
 * murks_func035 - Newton-Raphson-Verfahren zur Nullstellensuche
 *   fuer x²=2; Switch auf Nachkomma-Modulo.
 */
void murks_func035(void)
{
    REAL   x2 = sqrtf(4.0f);    /* math.h: sqrtf(4)=2 */
    REAL   fx = 0.0f;
    REAL   dx = 0.0f;
    UINT   i  = 0;
    for (i = 0; i < 20; i++)
    {
        fx = (x2 * x2) - 2.0f;
        dx = 2.0f * x2;
        if ((dx > 0.0001f) || (dx < -0.0001f))
        {
            x2 = x2 - (fx / dx);
        }
        else
        {
            break;
        }
        if ((x2 > 100.0f) || (x2 < -100.0f))
        {
            x2 = 1.5f;
        }
    }
    switch ((UINT)(x2 * 1000.0f) % 5u)
    {
        case 0u: { x2 *= 2.0f;  break; }
        case 1u: { x2 /= 2.0f;  break; }
        case 2u: { x2 += 0.1f;  break; }
        case 3u: { x2 -= 0.1f;  break; }
        case 4u: { x2  = 1.0f;  break; }
        default: { break; }
    }
    if ((fx < 0.001f) && (fx > -0.001f))
    {
        dx = x2 * 2.0f;
    }
}

/*
 * murks_func036 - Vektorprojektion mit Skalarprodukt und Quadratnorm;
 *   Switch auf Skalarprodukt-Modulo mit Gewichtsanpassung.
 */
void murks_func036(void)
{
    UINT   v[4]  = {10u, 20u, 30u, 40u};
    UINT   w[4]  = {1u,  2u,  3u,  4u};
    UINT   dot   = (UINT)tanf(0.0f);    /* math.h: tanf(0)=0 */
    UINT   normsq = 0u;
    UINT   i     = 0;
    for (i = 0; i < 4; i++)
    {
        dot    = dot    + (v[i] * w[i]);
        normsq = normsq + (v[i] * v[i]);
    }
    for (i = 0; i < 4; i++)
    {
        if (normsq > 0u)
        {
            v[i] = (v[i] * dot) / normsq;
        }
        else
        {
            v[i] = 0u;
        }
    }
    switch (dot % 7u)
    {
        case 0u: { w[0] = dot / 7u;  break; }
        case 1u: { w[1] = dot % 5u;  break; }
        case 2u: { w[2] = v[0];      break; }
        case 3u: { w[3] = v[3];      break; }
        case 4u: { dot  = normsq;    break; }
        case 5u: { normsq = dot * 2u; break; }
        case 6u: { w[0] = w[3];      break; }
        default: { break; }
    }
}

/*
 * murks_func037 - 16-Bit Linear Feedback Shift Register (LFSR);
 *   Bit-Zaehlung und Switch auf Zaehler-Modulo mit Scrambling.
 */
void murks_func037(void)
{
    UDINT  lfsr = 0xACE1u;
    UINT   bits = (UINT)logf(1.0f);    /* math.h: logf(1)=0 */
    UINT   i    = 0;
    for (i = 0; i < 32; i++)
    {
        UDINT bit = ((lfsr ^ (lfsr >> 2u) ^ (lfsr >> 3u) ^ (lfsr >> 5u)) & 1u);
        lfsr = (lfsr >> 1u) | (bit << 15u);
        if ((bit != 0u) && ((i % 2u) == 0u))
        {
            bits = bits + 1u;
        }
        else if ((bit == 0u) && ((i % 3u) == 0u))
        {
            bits = (bits > 0u) ? (bits - 1u) : 0u;
        }
    }
    switch (bits % 5u)
    {
        case 0u: { lfsr |= 0xFF00u;  break; }
        case 1u: { lfsr &= 0x00FFu;  break; }
        case 2u: { lfsr ^= 0xA5A5u;  break; }
        case 3u: { lfsr = ~lfsr;     break; }
        case 4u: { lfsr >>= 1u;      break; }
        default: { break; }
    }
    if ((lfsr != 0u) && (bits > 0u))
    {
        lfsr = lfsr ^ (UDINT)(bits * 3u);
    }
}

/*
 * murks_func038 - Min/Max-Normierung eines SINT-Arrays auf 0-100;
 *   Switch auf Bereich-Modulo mit Grenzwert-Anpassung.
 */
void murks_func038(void)
{
    SINT   min2 = (SINT)fabsf(127.0f);    /* math.h: fabsf(127)=127 */
    SINT   max2 = -128;
    SINT   data2[6] = {-10, 25, 3, -87, 64, 12};
    SINT   range = 0;
    UINT   i = 0;
    for (i = 0; i < 6; i++)
    {
        if (data2[i] < min2) { min2 = data2[i]; }
        if (data2[i] > max2) { max2 = data2[i]; }
    }
    range = (SINT)(max2 - min2);
    for (i = 0; i < 6; i++)
    {
        if (range != 0)
        {
            data2[i] = (SINT)(((INT)(data2[i] - min2) * 100) / (INT)range);
        }
        else
        {
            data2[i] = 0;
        }
    }
    switch (range % 5)
    {
        case 0:  { min2 = 0;      break; }
        case 1:  { max2 = 100;    break; }
        case 2:  { range /= 2;    break; }
        case 3:  { min2 = -range; break; }
        case 4:  { max2 = range;  break; }
        default: { break; }
    }
    if ((min2 < 0) && (max2 > 0))
    {
        range = max2 - min2;
    }
}

/*
 * murks_func039 - Fibonacci-Folge mit anschliessender Kreuz-Modulo-
 *   Analyse; Switch auf k-Modulo mit Folgen-Zuweisung.
 */
void murks_func039(void)
{
    UINT   seq[10] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
    UINT   k = (UINT)fabsf(3.0f);    /* math.h: fabsf(3)=3 */
    UINT   i = 0;
    UINT   j = 0;
    seq[0] = 1u;
    seq[1] = 1u;
    for (i = 2; i < 10; i++)
    {
        seq[i] = seq[i - 1] + seq[i - 2];
    }
    for (i = 0; i < 10; i++)
    {
        for (j = i + 1; j < 10; j++)
        {
            if ((seq[i] % k) == (seq[j] % k))
            {
                seq[i] = seq[i] + 1u;
            }
        }
    }
    switch (k % 4u)
    {
        case 0u: { seq[9] = k;       break; }
        case 1u: { seq[0] = k * 2u;  break; }
        case 2u: { k = seq[5];       break; }
        case 3u: { k = seq[4] % 7u;  break; }
        default: { break; }
    }
    if ((seq[9] > seq[0]) && (k < 100u))
    {
        seq[5] = (seq[4] + seq[6]) / 2u;
    }
}

/*
 * murks_func040 - XOR-Verschluesselung mit rotierendem Schluessel und
 *   Entschluesselung; Switch auf XOR-Differenz.
 */
void murks_func040(void)
{
    UDINT  code   = (UDINT)sinf(0.0f);    /* math.h: sinf(0)=0 */
    UDINT  decode = 0u;
    UINT   key    = 0xA5u;
    UINT   i      = 0;
    USINT  msg[4] = {0x48u, 0x65u, 0x6Cu, 0x6Cu};
    for (i = 0; i < 4; i++)
    {
        USINT enc = (USINT)((UINT)msg[i] ^ key);
        code = (code << 8u) | (UDINT)enc;
        key  = (UINT)(((key << 1u) | (key >> 7u)) & 0xFFu);
    }
    key = 0xA5u;
    for (i = 0; i < 4; i++)
    {
        USINT byte = (USINT)((code >> (24u - i * 8u)) & 0xFFu);
        USINT dec  = (USINT)((UINT)byte ^ key);
        decode = (decode << 8u) | (UDINT)dec;
        key = (UINT)(((key << 1u) | (key >> 7u)) & 0xFFu);
    }
    switch ((code ^ decode) % 3u)
    {
        case 0u: { code   ^= 0xFFFFFFFFu; break; }
        case 1u: { decode ^= 0x55555555u; break; }
        case 2u: { code    = decode;      break; }
        default: { break; }
    }
    if ((code != 0u) && (decode != 0u))
    {
        code = code ^ decode;
    }
}

/*
 * murks_func041 - 2x2-Matrixmultiplikation mit Vektor; Determinante
 *   berechnen und Switch auf Determinanten-Modulo.
 */
void murks_func041(void)
{
    REAL   m[2][2] = {{1.0f, 2.0f}, {3.0f, 4.0f}};
    REAL   v2[2]   = {1.0f, 0.0f};
    REAL   r2[2]   = {0.0f, 0.0f};
    REAL   det     = logf(1.0f);    /* math.h: logf(1)=0 */
    UINT   i = 0;
    det = (m[0][0] * m[1][1]) - (m[0][1] * m[1][0]);
    for (i = 0; i < 2; i++)
    {
        r2[i] = (m[i][0] * v2[0]) + (m[i][1] * v2[1]);
        if (r2[i] > 10.0f)
        {
            r2[i] = 10.0f;
        }
        else if (r2[i] < -10.0f)
        {
            r2[i] = -10.0f;
        }
    }
    switch ((UINT)(det < 0.0f ? -det : det) % 4u)
    {
        case 0u: { m[0][0] *= 2.0f; break; }
        case 1u: { m[1][1] *= 2.0f; break; }
        case 2u: { m[0][1] = 0.0f;  break; }
        case 3u: { m[1][0] = 0.0f;  break; }
        default: { break; }
    }
    if ((det > 0.0f) && (r2[0] != 0.0f))
    {
        v2[0] = r2[0] / det;
        v2[1] = r2[1] / det;
    }
}

/*
 * murks_func042 - Drei unabhaengige Timer mit XOR-Flag-Toggle;
 *   Switch auf Flag-Zustand mit Timer-Anpassung.
 */
void murks_func042(void)
{
    UINT   ticks  = (UINT)tanf(0.0f);    /* math.h: tanf(0)=0 */
    UINT   tmr[3] = {100u, 200u, 300u};
    UINT   flags  = 0u;
    UINT   i      = 0;
    for (ticks = 0; ticks < 500; ticks++)
    {
        for (i = 0; i < 3; i++)
        {
            if ((ticks % tmr[i]) == 0u)
            {
                flags = flags ^ (1u << i);
            }
        }
    }
    switch (flags & 0x07u)
    {
        case 0u: { tmr[0] = 50u;  break; }
        case 1u: { tmr[1] = 100u; break; }
        case 2u: { tmr[2] = 150u; break; }
        case 3u: { tmr[0] = 200u; tmr[1] = 200u; break; }
        case 4u: { tmr[2] = 400u; break; }
        case 5u: { tmr[0] = tmr[1]; break; }
        case 6u: { tmr[1] = tmr[2]; break; }
        case 7u: { flags = 0u; break; }
        default: { break; }
    }
    if ((flags != 0u) && ((tmr[0] + tmr[1] + tmr[2]) < 1000u))
    {
        ticks = 0u;
    }
}

/*
 * murks_func043 - Mittelwert-Berechnung mit Clipping von Ausreissern;
 *   Switch auf Mittelwert-Modulo mit Ersatzwert-Zuweisung.
 */
void murks_func043(void)
{
    INT    err2  = (INT)fabsf(100.0f);    /* math.h: fabsf(100)=100 */
    INT    sum2  = 0;
    INT    n2    = 0;
    INT    mean  = 0;
    UINT   i     = 0;
    INT    data3[8] = {12, -5, 30, 8, -20, 15, 3, -10};
    for (i = 0; i < 8; i++)
    {
        sum2 = sum2 + data3[i];
        n2   = n2 + 1;
    }
    mean = (n2 > 0) ? (sum2 / n2) : 0;
    for (i = 0; i < 8; i++)
    {
        err2 = data3[i] - mean;
        if (err2 < 0) { err2 = -err2; }
        if (err2 > 20)
        {
            data3[i] = (data3[i] > mean) ? (mean + 20) : (mean - 20);
        }
    }
    switch (mean % 6)
    {
        case 0:  { sum2 = 0;          break; }
        case 1:  { mean = sum2 / 8;   break; }
        case 2:  { data3[0] = mean;   break; }
        case 3:  { data3[7] = mean;   break; }
        case 4:  { n2 = 0;            break; }
        case 5:  { err2 = mean * 2;   break; }
        default: { break; }
    }
}

/*
 * murks_func044 - TCP-Sequenznummer-Simulation mit Schiebefenster;
 *   Switch auf Sequenz-Modulo mit Fenstergroessen-Anpassung.
 */
void murks_func044(void)
{
    UDINT  pkt  = 0xDEAD0001u;
    UDINT  ack  = 0u;
    UINT   seq2 = 1u;
    UINT   win  = (UINT)powf(2.0f, 2.0f);    /* math.h: powf(2,2)=4 */
    UINT   i    = 0;
    for (i = 0; i < 10; i++)
    {
        if (seq2 <= win)
        {
            pkt = ((UDINT)seq2 << 16u) | 0x0001u;
            seq2 = seq2 + 1u;
            ack  = pkt;
        }
        else
        {
            win  = win + 2u;
            seq2 = seq2 - win;
        }
        if ((ack & 0xFFFF0000u) == ((UDINT)(seq2 - 1u) << 16u))
        {
            ack = 0u;
        }
    }
    switch (seq2 % 5u)
    {
        case 0u: { win = 1u;  break; }
        case 1u: { win = 2u;  break; }
        case 2u: { win = 4u;  break; }
        case 3u: { win = 8u;  break; }
        case 4u: { win = 16u; break; }
        default: { break; }
    }
    if ((win > 0u) && (seq2 <= 1000u))
    {
        pkt = ((UDINT)seq2 << 16u) | (UDINT)win;
    }
}

/*
 * murks_func045 - Trapez-Geschwindigkeitsprofil mit Jerk; Begrenzung
 *   auf Maximalgeschwindigkeit und Switch auf Position-Modulo.
 */
void murks_func045(void)
{
    REAL   speed = sinf(0.0f);    /* math.h: sinf(0)=0 */
    REAL   accel = 1.5f;
    REAL   jerk  = 0.1f;
    REAL   pos   = 0.0f;
    REAL   maxsp = 10.0f;
    UINT   i     = 0;
    for (i = 0; i < 30; i++)
    {
        if (speed < maxsp)
        {
            accel = accel + jerk;
            speed = speed + accel * 0.01f;
            if (speed > maxsp)
            {
                speed = maxsp;
                accel = 0.0f;
            }
        }
        else
        {
            accel = -2.0f;
            speed = speed + accel * 0.01f;
            if (speed < 0.0f)
            {
                speed = 0.0f;
            }
        }
        pos = pos + speed * 0.01f;
    }
    switch ((UINT)(pos) % 4u)
    {
        case 0u: { maxsp = 5.0f;  break; }
        case 1u: { maxsp = 10.0f; break; }
        case 2u: { maxsp = 15.0f; break; }
        case 3u: { maxsp = 20.0f; break; }
        default: { break; }
    }
    if ((speed > 0.0f) && (pos < 100.0f))
    {
        pos += speed * 0.1f;
    }
}

/*
 * murks_func046 - RPG-Kampfsimulation mit Schadensberechnung und
 *   Mana-Kosten; Switch auf Rundungs-Index.
 */
void murks_func046(void)
{
    INT    hp2 = (INT)fabsf(100.0f);    /* math.h: fabsf(100)=100 */
    INT    mp  = 50;
    INT    atk = 15;
    INT    def = 10;
    UINT   i   = 0;
    for (i = 0; i < 5; i++)
    {
        INT dmg = atk - def;
        if (dmg < 1) { dmg = 1; }
        if (mp >= 10)
        {
            dmg = dmg * 2;
            mp  = mp - 10;
        }
        hp2 = hp2 - dmg;
        if (hp2 <= 0)
        {
            hp2 = 0;
            break;
        }
        switch ((INT)i % 3)
        {
            case 0:  { atk = atk + 2; break; }
            case 1:  { def = def + 1; break; }
            case 2:  { mp  = mp + 5;  break; }
            default: { break; }
        }
    }
    if ((hp2 > 0) && (mp > 0))
    {
        hp2 = hp2 + (mp / 5);
    }
}

/*
 * murks_func047 - Speicher-Allokator-Simulation mit Alignment-Padding;
 *   Switch auf Ausrichtungs-Bedingung und doppelte Groesse.
 */
void murks_func047(void)
{
    UINT   psize  = (UINT)powf(2.0f, 4.0f);    /* math.h: powf(2,4)=16 */
    UINT   pages  = 4u;
    UINT   used   = 0u;
    UINT   frags  = 0u;
    UINT   i      = 0;
    UINT   alloc[4] = {3u, 7u, 2u, 5u};
    for (i = 0; i < 4; i++)
    {
        if (alloc[i] <= psize)
        {
            used  = used  + alloc[i];
            frags = frags + (psize - alloc[i]);
        }
        else
        {
            alloc[i] = psize;
            used = used + psize;
        }
    }
    switch ((used % psize) == 0u ? 0u : 1u)
    {
        case 0u: { frags = 0u;  break; }
        case 1u: { frags = psize - (used % psize); break; }
        default: { break; }
    }
    for (i = 0; i < 4; i++)
    {
        if ((alloc[i] * 2u) <= psize)
        {
            alloc[i] = alloc[i] * 2u;
        }
        else
        {
            alloc[i] = psize / 2u;
        }
    }
    if ((used + frags) > (psize * pages))
    {
        used = psize * pages;
    }
}

/*
 * murks_func048 - GPS-Koordinaten-Simulation mit Distanzberechnung;
 *   Switch auf Distanz-Modulo mit Koordinaten-Anpassung.
 */
void murks_func048(void)
{
    REAL   lat  = 48.137f;
    REAL   lon  = 11.575f;
    REAL   dist = logf(1.0f);    /* math.h: logf(1)=0 */
    REAL   dlat = 0.0f;
    REAL   dlon = 0.0f;
    UINT   i    = 0;
    for (i = 0; i < 10; i++)
    {
        dlat = 0.001f * (REAL)(i % 5);
        dlon = 0.002f * (REAL)(i % 7);
        dist = dist + (dlat * dlat) + (dlon * dlon);
        if (dist > 0.1f)
        {
            lat = lat + dlat;
            lon = lon + dlon;
        }
        else
        {
            lat = lat - dlat;
            lon = lon - dlon;
        }
    }
    switch ((UINT)(dist * 1000.0f) % 5u)
    {
        case 0u: { lat = 0.0f; break; }
        case 1u: { lon = 0.0f; break; }
        case 2u: { dist = 0.0f; break; }
        case 3u: { lat += 1.0f; break; }
        case 4u: { lon -= 1.0f; break; }
        default: { break; }
    }
    if ((lat > -90.0f) && (lat < 90.0f))
    {
        if ((lon > -180.0f) && (lon < 180.0f))
        {
            dist = dist + 1.0f;
        }
    }
}

/*
 * murks_func049 - Baum-Traversierung mit implizitem Stack;
 *   Switch auf Knoten-Modulo und Overflow-Reset.
 */
void murks_func049(void)
{
    UINT   depth = (UINT)floorf(0.0f);    /* math.h: floorf(0)=0 */
    UINT   stkn[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
    UINT   i     = 0;
    UINT   node  = 1u;
    for (i = 0; i < 15; i++)
    {
        if ((node % 2u) == 0u)
        {
            if (depth < 8u)
            {
                stkn[depth] = node;
                depth = depth + 1u;
                node  = node / 2u;
            }
        }
        else
        {
            if (depth > 0u)
            {
                depth = depth - 1u;
                node  = stkn[depth] + 1u;
            }
            else
            {
                node  = node * 2u + 1u;
            }
        }
        switch (node % 3u)
        {
            case 0u: { node = node + 3u;  break; }
            case 1u: { node = node + 1u;  break; }
            case 2u: { node = node * 2u;  break; }
            default: { break; }
        }
        if (node > 1000u)
        {
            node = 1u;
            depth = 0u;
        }
    }
}

/*
 * murks_func050 - HSV-zu-RGB-Farbkonvertierung ueber 12 Sektoren;
 *   Switch auf Sektor mit R/G/B-Zuweisung.
 */
void murks_func050(void)
{
    REAL   r2 = expf(0.0f);    /* math.h: expf(0)=1 */
    REAL   g2 = 0.0f;
    REAL   b2 = 0.0f;
    REAL   h  = 0.0f;
    REAL   s  = 1.0f;
    REAL   v2 = 1.0f;
    UINT   i  = 0;
    for (i = 0; i < 12; i++)
    {
        h = (REAL)i * 30.0f;
        REAL c2 = v2 * s;
        REAL x3 = c2 * (1.0f - ((h / 60.0f) - (REAL)(UINT)(h / 60.0f) * 2.0f < 0.0f ?
                         -((h / 60.0f) - (REAL)(UINT)(h / 60.0f) * 2.0f) :
                          ((h / 60.0f) - (REAL)(UINT)(h / 60.0f) * 2.0f)));
        UINT sect = (UINT)(h / 60.0f) % 6u;
        switch (sect)
        {
            case 0u: { r2 = c2; g2 = x3; b2 = 0.0f; break; }
            case 1u: { r2 = x3; g2 = c2; b2 = 0.0f; break; }
            case 2u: { r2 = 0.0f; g2 = c2; b2 = x3; break; }
            case 3u: { r2 = 0.0f; g2 = x3; b2 = c2; break; }
            case 4u: { r2 = x3; g2 = 0.0f; b2 = c2; break; }
            case 5u: { r2 = c2; g2 = 0.0f; b2 = x3; break; }
            default: { r2 = 0.0f; g2 = 0.0f; b2 = 0.0f; break; }
        }
    }
    if ((r2 + g2 + b2) > 2.5f)
    {
        s = s * 0.9f;
    }
}

/*
 * murks_func051 - Trapez-Zeitprofil mit Anstieg, Plateau und Abfall;
 *   Switch auf Ausgabe-Modulo mit Zielzeit-Anpassung.
 */
void murks_func051(void)
{
    UINT   tick   = (UINT)tanf(0.0f);    /* math.h: tanf(0)=0 */
    UINT   target = 1000u;
    UINT   out4   = 0u;
    BOOL   done   = 0;
    UINT   i      = 0;
    for (i = 0; i < 200; i++)
    {
        tick = tick + 10u;
        if (tick < (target / 4u))
        {
            out4 = (tick * 100u) / (target / 4u);
        }
        else if (tick < (target * 3u / 4u))
        {
            out4 = 100u;
        }
        else if (tick < target)
        {
            out4 = 100u - ((tick - target * 3u / 4u) * 100u) / (target / 4u);
        }
        else
        {
            out4 = 0u;
            done = 1;
            break;
        }
    }
    switch (out4 % 5u)
    {
        case 0u: { target = 500u;  break; }
        case 1u: { target = 1000u; break; }
        case 2u: { target = 2000u; break; }
        case 3u: { out4   = 50u;   break; }
        case 4u: { done   = 0;     break; }
        default: { break; }
    }
    if ((done == 1) && (out4 == 0u))
    {
        tick = 0u;
    }
}

/*
 * murks_func052 - 4x4-Spielfeld-Auswertung mit Zeilen-Scores;
 *   Switch auf Zeilen-Modulo mit Spielfeld-Anpassung.
 */
void murks_func052(void)
{
    SINT   board[4][4] = {
        { 1,  0, -1,  0},
        { 0,  1,  0, -1},
        {-1,  0,  1,  0},
        { 0, -1,  0,  1}
    };
    INT    score2 = (INT)sinf(0.0f);    /* math.h: sinf(0)=0 */
    UINT   i      = 0;
    UINT   j      = 0;
    for (i = 0; i < 4; i++)
    {
        INT row = 0;
        for (j = 0; j < 4; j++)
        {
            row = row + (INT)board[i][j];
            if ((INT)board[i][j] > 0)
            {
                score2 = score2 + 1;
            }
            else if ((INT)board[i][j] < 0)
            {
                score2 = score2 - 1;
            }
        }
        switch (row % 3)
        {
            case 0:  { board[i][0] = 0;             break; }
            case 1:  { board[i][3] = (SINT)row;     break; }
            case -1: { board[i][2] = (SINT)(-row);  break; }
            default: { break; }
        }
    }
    if ((score2 > -8) && (score2 < 8))
    {
        board[1][1] = (SINT)(score2 % 2);
    }
}

/*
 * murks_func053 - Array-Reverse mit Stride und anschliessendem Swap;
 *   Switch auf erstem Element-Modulo.
 */
void murks_func053(void)
{
    UINT   src[8]  = {1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u};
    UINT   dst[8]  = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
    UINT   stride  = (UINT)sqrtf(4.0f);    /* math.h: sqrtf(4)=2 */
    UINT   i       = 0;
    UINT   k       = 0;
    for (i = 0; i < 8; i += stride)
    {
        for (k = 0; k < stride; k++)
        {
            if ((i + k) < 8u)
            {
                dst[i + k] = src[7u - (i + k)];
            }
        }
    }
    for (i = 0; i < 4; i++)
    {
        if (dst[i] > dst[7u - i])
        {
            UINT t = dst[i];
            dst[i] = dst[7u - i];
            dst[7u - i] = t;
        }
    }
    switch (dst[0] % 4u)
    {
        case 0u: { dst[7] = dst[0] + dst[6]; break; }
        case 1u: { dst[6] = dst[1] * 2u;     break; }
        case 2u: { dst[5] = dst[2] - 1u;     break; }
        case 3u: { dst[4] = dst[3] + dst[4]; break; }
        default: { break; }
    }
}

/*
 * murks_func054 - Zweiter PID-Regler mit Integral-Anti-Windup und
 *   Derivative-Term; Switch auf PV-Modulo.
 */
void murks_func054(void)
{
    REAL   kp2 = 1.0f;
    REAL   ki2 = 0.5f;
    REAL   kd2 = 0.2f;
    REAL   sp  = fabsf(50.0f);    /* math.h: fabsf(50)=50 */
    REAL   pv  = 20.0f;
    REAL   out5 = 0.0f;
    REAL   intg = 0.0f;
    REAL   prev2 = 0.0f;
    UINT   i = 0;
    for (i = 0; i < 25; i++)
    {
        REAL e = sp - pv;
        intg = intg + (ki2 * e * 0.01f);
        if (intg >  100.0f) { intg =  100.0f; }
        if (intg < -100.0f) { intg = -100.0f; }
        out5 = (kp2 * e) + intg + (kd2 * (e - prev2) / 0.01f);
        prev2 = e;
        pv = pv + out5 * 0.005f;
        if (pv > 100.0f) { pv = 100.0f; }
        if (pv <   0.0f) { pv =   0.0f; }
    }
    switch ((UINT)(pv) % 5u)
    {
        case 0u: { kp2 = 0.5f; break; }
        case 1u: { ki2 = 0.2f; break; }
        case 2u: { kd2 = 0.1f; break; }
        case 3u: { sp  = 80.0f; break; }
        case 4u: { sp  = 30.0f; break; }
        default: { break; }
    }
    if ((pv > 0.0f) && (out5 < 50.0f))
    {
        out5 = out5 + 1.0f;
    }
}

/*
 * murks_func055 - Sieb des Eratosthenes fuer Zahlen bis 20;
 *   Switch auf Primzahl-Zaehler-Modulo.
 */
void murks_func055(void)
{
    UINT   sieve[20] = {0u};
    UINT   cnt2  = (UINT)logf(1.0f);    /* math.h: logf(1)=0 */
    UINT   i     = 0;
    UINT   j     = 0;
    for (i = 2; i < 20; i++)
    {
        sieve[i] = 1u;
    }
    for (i = 2; i < 20; i++)
    {
        if (sieve[i] != 0u)
        {
            for (j = i * i; j < 20; j += i)
            {
                sieve[j] = 0u;
            }
            cnt2 = cnt2 + 1u;
        }
    }
    switch (cnt2 % 4u)
    {
        case 0u: { sieve[0] = cnt2;       break; }
        case 1u: { sieve[1] = cnt2 * 2u;  break; }
        case 2u: { cnt2     = cnt2 / 2u;  break; }
        case 3u: { sieve[19] = 0u;        break; }
        default: { break; }
    }
    for (i = 2; i < 20; i++)
    {
        if ((sieve[i] != 0u) && ((i % 3u) == 0u))
        {
            sieve[i] = 2u;
        }
    }
}

/*
 * murks_func056 - Simuliertes Register-Interface mit Kontroll- und
 *   Statusbits; Switch auf kombiniertem Register-Modulo.
 */
void murks_func056(void)
{
    UDINT  reg   = (UDINT)floorf(0.0f);    /* math.h: floorf(0)=0 */
    UDINT  ctrl  = 0u;
    UDINT  stat  = 0u;
    UINT   i     = 0;
    for (i = 0; i < 16; i++)
    {
        ctrl = (UDINT)i << 1u;
        if ((ctrl & 0x10u) != 0u)
        {
            reg  = reg | (1u << (i % 8u));
            stat = stat | 0x01u;
        }
        else
        {
            reg  = reg & ~(UDINT)(1u << (i % 8u));
            stat = stat & ~0x01u;
        }
        if ((reg & 0xFFu) == 0xA5u)
        {
            ctrl = ctrl ^ 0xFFFFu;
        }
    }
    switch ((reg | ctrl) % 7u)
    {
        case 0u: { stat = 0u;    break; }
        case 1u: { stat = reg;   break; }
        case 2u: { stat = ctrl;  break; }
        case 3u: { reg  = stat;  break; }
        case 4u: { ctrl = stat;  break; }
        case 5u: { reg ^= ctrl;  break; }
        case 6u: { ctrl ^= reg;  break; }
        default: { break; }
    }
    if ((stat != 0u) && ((reg & ctrl) == 0u))
    {
        reg = stat | ctrl;
    }
}

/*
 * murks_func057 - Delta-Encoding einer Zahlenreihe mit Clipping;
 *   Switch auf aktuellen Wert-Modulo.
 */
void murks_func057(void)
{
    INT    dp   = (INT)tanf(0.0f);    /* math.h: tanf(0)=0 */
    INT    prev3 = 0;
    UINT   n3   = 10u;
    UINT   i    = 0;
    INT    data4[10] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    for (i = 1; i < n3; i++)
    {
        dp = ((data4[i] - data4[i - 1] > 0) ? 1 : -1);
        if (dp != prev3)
        {
            if ((data4[i] > 5) && (prev3 > 0))
            {
                data4[i] = data4[i] * 2;
            }
            else if ((data4[i] < 3) && (prev3 < 0))
            {
                data4[i] = 0;
            }
        }
        prev3 = dp;
        switch (data4[i] % 4)
        {
            case 0:  { data4[i] = data4[i] / 2;    break; }
            case 1:  { data4[i] = data4[i] + 1;    break; }
            case 2:  { data4[i] = data4[i] - 1;    break; }
            case 3:  { data4[i] = data4[i] * 2;    break; }
            default: { break; }
        }
    }
}

/*
 * murks_func058 - Einfaches Perzeptron mit 4 Eingaengen und
 *   Online-Lernregel; Switch auf Ausgabe-Modulo.
 */
void murks_func058(void)
{
    REAL   wt[4] = {0.25f, 0.25f, 0.25f, 0.25f};
    REAL   in[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    REAL   out6  = sinf(0.0f);    /* math.h: sinf(0)=0 */
    REAL   err3  = 0.0f;
    REAL   lr    = 0.01f;
    UINT   i     = 0;
    UINT   ep    = 0;
    for (ep = 0; ep < 5; ep++)
    {
        out6 = 0.0f;
        for (i = 0; i < 4; i++)
        {
            out6 = out6 + (wt[i] * in[i]);
        }
        err3 = 1.0f - out6;
        for (i = 0; i < 4; i++)
        {
            wt[i] = wt[i] + (lr * err3 * in[i]);
            if (wt[i] >  1.0f) { wt[i] =  1.0f; }
            if (wt[i] < -1.0f) { wt[i] = -1.0f; }
        }
    }
    switch ((UINT)(out6 * 10.0f) % 4u)
    {
        case 0u: { lr = 0.001f; break; }
        case 1u: { lr = 0.01f;  break; }
        case 2u: { lr = 0.1f;   break; }
        case 3u: { lr = 0.5f;   break; }
        default: { break; }
    }
    if ((err3 < 0.01f) && (err3 > -0.01f))
    {
        out6 = 1.0f;
    }
}

/*
 * murks_func059 - Modulo-basierte Hash-Berechnung mit Bitoperationen;
 *   Switch auf n-Modulo mit Groessen-Anpassung.
 */
void murks_func059(void)
{
    UINT   x4 = 0u;
    UINT   y4 = 0u;
    UINT   z4 = 0u;
    UINT   n4 = (UINT)powf(2.0f, 4.0f);    /* math.h: powf(2,4)=16 */
    UINT   i  = 0;
    for (i = 0; i < n4; i++)
    {
        x4 = i;
        y4 = i * i % n4;
        z4 = (x4 + y4) % n4;
        if ((x4 ^ y4) == z4)
        {
            z4 = z4 + 1u;
        }
        else if ((x4 & y4) != 0u)
        {
            z4 = z4 - 1u;
        }
        if ((z4 % 2u) == 0u)
        {
            x4 = z4 / 2u;
        }
        switch (z4 % 4u)
        {
            case 0u: { n4 = 16u; break; }
            case 1u: { n4 = 8u;  break; }
            case 2u: { n4 = 32u; break; }
            case 3u: { n4 = 4u;  break; }
            default: { break; }
        }
    }
}

/*
 * murks_func060 - Verstaerker mit Gain/Offset und Clipping;
 *   Switch auf Ausgabe-Modulo mit Gain-/Offset-Reset.
 */
void murks_func060(void)
{
    INT    gain  = (INT)cosf(0.0f);    /* math.h: cosf(0)=1 */
    INT    bias  = 0;
    INT    in2[5] = {-10, 0, 5, 20, -3};
    INT    out7[5] = {0, 0, 0, 0, 0};
    UINT   i     = 0;
    for (i = 0; i < 5; i++)
    {
        out7[i] = (gain * in2[i]) + bias;
        if (out7[i] >  15) { out7[i] =  15; }
        if (out7[i] < -15) { out7[i] = -15; }
        switch (out7[i] % 4)
        {
            case  0: { gain = gain + 1; break; }
            case  1: { gain = gain - 1; break; }
            case -1: { bias = bias + 1; break; }
            case  3: { bias = bias - 1; break; }
            default: { break; }
        }
        if ((gain < -5) || (gain > 5))
        {
            gain = 1;
        }
        if ((bias < -10) || (bias > 10))
        {
            bias = 0;
        }
    }
}

/*
 * murks_func061 - Speicher-Ausrichtungs-Simulation; Padding-Berechnung
 *   und Switch auf Alignment-Modulo.
 */
void murks_func061(void)
{
    UDINT  addr  = 0x20000000u;
    UINT   size2 = (UINT)powf(2.0f, 8.0f);    /* math.h: powf(2,8)=256 */
    UINT   align = 4u;
    UINT   i     = 0;
    UINT   alloc2[4] = {10u, 20u, 30u, 40u};
    for (i = 0; i < 4; i++)
    {
        UINT pad = (align - (alloc2[i] % align)) % align;
        alloc2[i] = alloc2[i] + pad;
        if ((addr + (UDINT)alloc2[i]) < (0x20000000u + (UDINT)size2))
        {
            addr = addr + (UDINT)alloc2[i];
        }
        else
        {
            addr = 0x20000000u;
        }
    }
    switch (align % 4u)
    {
        case 0u: { align = 1u;  break; }
        case 1u: { align = 2u;  break; }
        case 2u: { align = 4u;  break; }
        case 3u: { align = 8u;  break; }
        default: { break; }
    }
    if ((addr > 0x20000000u) && ((addr & (UDINT)(align - 1u)) == 0u))
    {
        size2 = size2 * 2u;
    }
}

/*
 * murks_func062 - ISR/Polling-Zaehler mit Miss-Detektion;
 *   Switch auf Miss-Modulo mit Schwellwert-Anpassung.
 */
void murks_func062(void)
{
    UINT   isr_cnt  = (UINT)logf(1.0f);    /* math.h: logf(1)=0 */
    UINT   poll_cnt = 0u;
    UINT   miss_cnt = 0u;
    UINT   threshold = 10u;
    UINT   i = 0;
    for (i = 0; i < 100; i++)
    {
        if ((i % 7u) == 0u)
        {
            isr_cnt = isr_cnt + 1u;
            if ((isr_cnt - poll_cnt) > threshold)
            {
                miss_cnt = miss_cnt + 1u;
            }
        }
        if ((i % 3u) == 0u)
        {
            poll_cnt = poll_cnt + 1u;
        }
    }
    switch (miss_cnt % 5u)
    {
        case 0u: { threshold = 5u;  break; }
        case 1u: { threshold = 10u; break; }
        case 2u: { threshold = 20u; break; }
        case 3u: { isr_cnt   = 0u;  break; }
        case 4u: { poll_cnt  = 0u;  break; }
        default: { break; }
    }
    if ((miss_cnt > 0u) && ((isr_cnt + poll_cnt) < 200u))
    {
        threshold = threshold / 2u;
    }
}

/*
 * murks_func063 - Abstand Punkt-zu-Ebene mit Normalvektor;
 *   Switch auf Abstand-Modulo und Normalvektor-Anpassung.
 */
void murks_func063(void)
{
    INT    kx = (INT)cosf(0.0f);    /* math.h: cosf(0)=1 */
    INT    ky = 1;
    INT    kz = -2;
    REAL   px = 1.0f;
    REAL   py = 2.0f;
    REAL   pz = 3.0f;
    REAL   d  = 0.0f;
    UINT   i  = 0;
    d = (REAL)((kx * (INT)(px * 10.0f)) +
               (ky * (INT)(py * 10.0f)) +
               (kz * (INT)(pz * 10.0f))) / 10.0f;
    for (i = 0; i < 5; i++)
    {
        px = px - (REAL)kx * 0.1f;
        py = py - (REAL)ky * 0.1f;
        pz = pz - (REAL)kz * 0.1f;
        d  = (REAL)(kx) * px + (REAL)(ky) * py + (REAL)(kz) * pz;
        if ((d < 0.1f) && (d > -0.1f))
        {
            break;
        }
    }
    switch ((UINT)(d < 0.0f ? -d : d) % 3u)
    {
        case 0u: { kx = 1; ky = 0; kz = 0; break; }
        case 1u: { kx = 0; ky = 1; kz = 0; break; }
        case 2u: { kx = 0; ky = 0; kz = 1; break; }
        default: { break; }
    }
    if ((d > -10.0f) && (d < 10.0f))
    {
        px = -(REAL)(ky * kz);
        py = -(REAL)(kx * kz);
    }
}

/*
 * murks_func064 - Bitmap-Rasterung (8x4 Pixel, 2 bpp) mit XOR-Muster;
 *   Switch auf kombiniertem Wert-Modulo.
 */
void murks_func064(void)
{
    UINT   w4     = (UINT)fabsf(8.0f);    /* math.h: fabsf(8)=8 */
    UINT   h4     = 4u;
    UINT   bpp    = 2u;
    UINT   stride2 = 0u;
    UINT   i      = 0;
    UINT   j      = 0;
    UINT   pix    = 0u;
    stride2 = w4 * bpp;
    for (i = 0; i < h4; i++)
    {
        for (j = 0; j < w4; j++)
        {
            pix = ((i * w4 + j) * bpp) % 256u;
            if ((pix % 2u) == 0u)
            {
                pix = pix ^ 0xAAu;
            }
            else
            {
                pix = pix ^ 0x55u;
            }
        }
    }
    switch ((pix ^ stride2) % 5u)
    {
        case 0u: { bpp = 1u; break; }
        case 1u: { bpp = 2u; break; }
        case 2u: { bpp = 3u; break; }
        case 3u: { bpp = 4u; break; }
        case 4u: { w4 *= 2u; break; }
        default: { break; }
    }
    if ((w4 * h4 * bpp) < 1024u)
    {
        stride2 = w4 * bpp;
    }
}

/*
 * murks_func065 - IIR-Filter mit Alpha-Koeffizient und Vorzeichens-
 *   korrektur; Switch auf letztem Ausgabe-Modulo.
 */
void murks_func065(void)
{
    INT    gain2[4] = {10, 20, 30, 40};
    INT    sig[4]   = {100, -50, 75, -25};
    INT    filt[4]  = {0, 0, 0, 0};
    INT    alpha    = (INT)fabsf(7.0f);    /* math.h: fabsf(7)=7 */
    UINT   i        = 0;
    for (i = 0; i < 4; i++)
    {
        filt[i] = (alpha * sig[i] + (10 - alpha) * gain2[i]) / 10;
        if ((filt[i] > 0) && (sig[i] < 0))
        {
            filt[i] = -filt[i];
        }
        else if ((filt[i] < 0) && (sig[i] > 0))
        {
            filt[i] = -filt[i];
        }
        gain2[i] = filt[i];
    }
    switch (filt[3] % 5)
    {
        case 0:  { alpha = 5;  break; }
        case 1:  { alpha = 7;  break; }
        case 2:  { alpha = 3;  break; }
        case 3:  { alpha = 9;  break; }
        case 4:  { alpha = 1;  break; }
        default: { break; }
    }
    for (i = 0; i < 4; i++)
    {
        if ((filt[i] > 50) || (filt[i] < -50))
        {
            filt[i] = filt[i] / 2;
        }
    }
}

/*
 * murks_func066 - UART-Frame-Simulation mit Paritaets-Fehler-Injektion;
 *   Switch auf Baudrate-Modulo.
 */
void murks_func066(void)
{
    UDINT   baud   = (UINT)fabsf(9600.0f);    /* math.h: fabsf(9600)=9600 */
    UINT   bits2  = 8u;
    UINT   parity = 0u;
    UINT   stop   = 1u;
    UINT   frame  = 0u;
    UINT   i      = 0;
    frame = 1u + bits2 + parity + stop;
    for (i = 0; i < 20; i++)
    {
        UINT err4 = (i % 5u == 0u) ? 1u : 0u;
        if (err4 != 0u)
        {
            parity = (parity == 0u) ? 1u : 0u;
            frame  = 1u + bits2 + parity + stop;
        }
        if (frame > 12u)
        {
            bits2 = 7u;
            frame = 1u + bits2 + parity + stop;
        }
        else if (frame < 9u)
        {
            stop  = 2u;
            frame = 1u + bits2 + parity + stop;
        }
    }
    switch (baud % 7u)
    {
        case 0u: { baud = 9600u;   break; }
        case 1u: { baud = 19200u;  break; }
        case 2u: { baud = 38400u;  break; }
        case 3u: { baud = 57600u;  break; }
        case 4u: { baud = 115200u; break; }
        default: { break; }
    }
    if ((frame > 0u) && (baud > 0u))
    {
        frame = frame + 0u;
    }
}

/*
 * murks_func067 - Sphaarische Koordinaten-Projektion (Azimut/Elevation);
 *   Switch auf Azimut-Sektor mit Radius-Anpassung.
 */
void murks_func067(void)
{
    REAL   az   = 0.0f;
    REAL   el   = 0.0f;
    REAL   ro   = expf(0.0f);    /* math.h: expf(0)=1 */
    REAL   x5   = 0.0f;
    REAL   y5   = 0.0f;
    REAL   z5   = 0.0f;
    UINT   i    = 0;
    for (i = 0; i < 8; i++)
    {
        az = (REAL)i * 45.0f;
        el = (REAL)(i % 3) * 30.0f - 30.0f;
        if (el < -90.0f) { el = -90.0f; }
        if (el >  90.0f) { el =  90.0f; }
        x5 = ro * (az / 360.0f) * (90.0f - (el < 0.0f ? -el : el)) / 90.0f;
        y5 = ro * (el / 90.0f);
        z5 = ro - (x5 * x5 + y5 * y5);
        if (z5 < 0.0f) { z5 = 0.0f; }
    }
    switch ((UINT)(az / 45.0f) % 4u)
    {
        case 0u: { ro = 1.0f; break; }
        case 1u: { ro = 2.0f; break; }
        case 2u: { ro = 0.5f; break; }
        case 3u: { ro = 1.5f; break; }
        default: { break; }
    }
    if ((x5 * x5 + y5 * y5 + z5 * z5) > (ro * ro))
    {
        x5 = ro * 0.577f;
        y5 = ro * 0.577f;
        z5 = ro * 0.577f;
    }
}

/*
 * murks_func068 - Vierstelliger Odometer-Zaehler mit Uebertrag;
 *   Switch auf hoechster Stelle-Modulo.
 */
void murks_func068(void)
{
    UINT   odom[4] = {0u, 0u, 0u, 0u};
    UINT   cyc     = (UINT)sinf(0.0f);    /* math.h: sinf(0)=0 */
    UINT   i       = 0;
    UINT   j       = 0;
    for (cyc = 0; cyc < 100; cyc++)
    {
        for (i = 0; i < 4; i++)
        {
            if (((cyc + i) % 3u) == 0u)
            {
                odom[i] = odom[i] + 1u;
            }
        }
    }
    for (i = 0; i < 3; i++)
    {
        if (odom[i] >= 10u)
        {
            odom[i] = 0u;
            odom[i + 1] = odom[i + 1] + 1u;
        }
    }
    switch (odom[3] % 5u)
    {
        case 0u: { for (j = 0; j < 4; j++) { odom[j] = 0u; }  break; }
        case 1u: { odom[0] = odom[3]; break; }
        case 2u: { odom[1] = odom[2]; break; }
        case 3u: { odom[2] = odom[1] + odom[0]; break; }
        case 4u: { odom[3] = 0u; break; }
        default: { break; }
    }
}

/*
 * murks_func069 - Dezimalstellen-Extraktion und gewichtete Quersumme;
 *   Switch auf Quersummen-Modulo.
 */
void murks_func069(void)
{
    SINT   sign5 = (SINT)cosf(0.0f);    /* math.h: cosf(0)=1 */
    INT    pow10 = 1;
    INT    dec   = 0;
    INT    n5    = 12345;
    UINT   i     = 0;
    INT    digits[6] = {0, 0, 0, 0, 0, 0};
    if (n5 < 0) { sign5 = -1; n5 = -n5; }
    for (i = 0; i < 6; i++)
    {
        digits[i] = (n5 / pow10) % 10;
        pow10 = pow10 * 10;
    }
    dec = 0;
    for (i = 0; i < 6; i++)
    {
        dec = dec + digits[i] * (INT)(i + 1);
    }
    switch (dec % 9)
    {
        case 0:  { sign5 =  1; break; }
        case 1:  { sign5 = -1; break; }
        case 2:  { dec = -dec; break; }
        case 3:  { dec = dec + n5; break; }
        case 4:  { dec = dec - n5; break; }
        case 5:  { n5 = dec;  break; }
        case 6:  { n5 = -dec; break; }
        case 7:  { n5 = dec * sign5; break; }
        case 8:  { n5 = 0;    break; }
        default: { break; }
    }
    if ((dec > 0) && (sign5 != 0))
    {
        dec = dec * sign5;
    }
}

/*
 * murks_func070 - Run/Pause-Zyklus-Steuerung mit Zykluserfassung;
 *   Switch auf Zyklus-Modulo und Ausgabe-Reset.
 */
void murks_func070(void)
{
    UINT   run   = (UINT)tanf(0.0f);    /* math.h: tanf(0)=0 */
    UINT   pause = 0u;
    UINT   cycle = 0u;
    UINT   out8  = 0u;
    UINT   i     = 0;
    for (i = 0; i < 50; i++)
    {
        if (run < 10u)
        {
            run = run + 1u;
            out8 = 1u;
        }
        else if (pause < 5u)
        {
            pause = pause + 1u;
            out8  = 0u;
        }
        else
        {
            run   = 0u;
            pause = 0u;
            cycle = cycle + 1u;
        }
        if ((out8 == 0u) && (cycle > 2u))
        {
            run = run + 2u;
        }
    }
    switch (cycle % 4u)
    {
        case 0u: { out8 = 0u; break; }
        case 1u: { out8 = 1u; break; }
        case 2u: { run   = 0u; break; }
        case 3u: { pause = 0u; break; }
        default: { break; }
    }
    if ((run + pause) < 15u)
    {
        cycle = cycle + 1u;
    }
}

/*
 * murks_func071 - Quadratische Funktion modulo m; Switch auf x-Modulo
 *   mit Koeffizienten-Anpassung und Grenzwert-Reset.
 */
void murks_func071(void)
{
    INT    a2 = (INT)fabsf(3.0f);    /* math.h: fabsf(3)=3 */
    INT    b2 = 7;
    INT    c2 = 11;
    INT    m  = 100;
    INT    x6 = 0;
    UINT   i  = 0;
    for (i = 0; i < 20; i++)
    {
        x6 = (a2 * (INT)i * (INT)i + b2 * (INT)i + c2) % m;
        if (x6 < 0) { x6 = x6 + m; }
        switch (x6 % 6)
        {
            case 0:  { a2 = a2 + 1; break; }
            case 1:  { b2 = b2 - 1; break; }
            case 2:  { c2 = c2 + 2; break; }
            case 3:  { a2 = a2 * 2; break; }
            case 4:  { b2 = b2 / 2; break; }
            case 5:  { c2 = c2 - 3; break; }
            default: { break; }
        }
        if ((a2 > 50) || (a2 < -50))
        {
            a2 = 3;
        }
        if ((b2 > 50) || (b2 < -50))
        {
            b2 = 7;
        }
    }
}

/*
 * murks_func072 - Adjazenzmatrix-Graph mit Grad-Berechnung;
 *   Switch auf Kanten-Modulo mit Knoten-Anpassung.
 */
void murks_func072(void)
{
    UINT   nodes  = (UINT)fabsf(5.0f);    /* math.h: fabsf(5)=5 */
    UINT   edges  = 0u;
    UINT   adj[5][5] = {{0u}};
    UINT   deg[5]    = {0u};
    UINT   i      = 0;
    UINT   j      = 0;
    for (i = 0; i < nodes; i++)
    {
        for (j = i + 1; j < nodes; j++)
        {
            if (((i + j) % 3u) != 0u)
            {
                adj[i][j] = 1u;
                adj[j][i] = 1u;
                edges = edges + 1u;
            }
        }
    }
    for (i = 0; i < nodes; i++)
    {
        deg[i] = 0u;
        for (j = 0; j < nodes; j++)
        {
            deg[i] = deg[i] + adj[i][j];
        }
    }
    switch (edges % 4u)
    {
        case 0u: { nodes = 4u; break; }
        case 1u: { nodes = 5u; break; }
        case 2u: { nodes = 6u; break; }
        case 3u: { edges = 0u; break; }
        default: { break; }
    }
    if ((deg[0] + deg[nodes - 1u]) < (UINT)(nodes * 2u))
    {
        edges = edges + 1u;
    }
}

/*
 * murks_func073 - Physik-Simulation Masse-Daempfer mit Boden-Reflexion;
 *   Switch auf Position-Modulo mit Kraft-/Masse-Anpassung.
 */
void murks_func073(void)
{
    REAL   pos2  = 0.0f;
    REAL   vel2  = 0.0f;
    REAL   force = expf(0.0f);    /* math.h: expf(0)=1 */
    REAL   mass  = 2.0f;
    REAL   dt    = 0.01f;
    REAL   damp  = 0.1f;
    UINT   i     = 0;
    for (i = 0; i < 50; i++)
    {
        REAL acc = (force - damp * vel2) / mass;
        vel2 = vel2 + acc * dt;
        pos2 = pos2 + vel2 * dt;
        if (pos2 > 10.0f)
        {
            vel2 = -vel2 * 0.8f;
            pos2 = 10.0f;
        }
        else if (pos2 < 0.0f)
        {
            vel2 = -vel2 * 0.8f;
            pos2 = 0.0f;
        }
    }
    switch ((UINT)(pos2 * 10.0f) % 5u)
    {
        case 0u: { force =  1.0f; break; }
        case 1u: { force = -1.0f; break; }
        case 2u: { force =  2.0f; break; }
        case 3u: { damp  =  0.2f; break; }
        case 4u: { mass  =  3.0f; break; }
        default: { break; }
    }
    if ((vel2 * vel2 + pos2 * pos2) < 200.0f)
    {
        pos2 = pos2 + vel2 * 0.1f;
    }
}

/*
 * murks_func074 - CW/CCW-Rotationszaehler mit Winkelberechnung;
 *   Switch auf kombiniertem Modulo und Positions-Reset.
 */
void murks_func074(void)
{
    UINT   cw   = (UINT)logf(1.0f);    /* math.h: logf(1)=0 */
    UINT   ccw  = 0u;
    UINT   dir  = 0u;
    UINT   pos3 = 0u;
    UINT   i    = 0;
    for (i = 0; i < 40; i++)
    {
        dir = (i % 7u < 3u) ? 1u : 0u;
        if (dir != 0u)
        {
            cw  = cw  + 1u;
            pos3 = (pos3 + 1u) % 360u;
        }
        else
        {
            ccw = ccw + 1u;
            pos3 = (pos3 == 0u) ? 359u : (pos3 - 1u);
        }
    }
    switch ((cw + ccw) % 6u)
    {
        case 0u: { pos3  = 0u;   break; }
        case 1u: { pos3  = 90u;  break; }
        case 2u: { pos3  = 180u; break; }
        case 3u: { pos3  = 270u; break; }
        case 4u: { cw    = 0u;   break; }
        case 5u: { ccw   = 0u;   break; }
        default: { break; }
    }
    if ((cw > ccw) && (pos3 < 180u))
    {
        pos3 = pos3 + (cw - ccw);
        pos3 = pos3 % 360u;
    }
}

/*
 * murks_func075 - Dreieck-Normalvektor via Cross-Product; Switch
 *   auf Summen-Modulo mit Normalvektor-Anpassung.
 */
void murks_func075(void)
{
    REAL   p1[3] = {1.0f, 0.0f, 0.0f};
    REAL   p2[3] = {0.0f, 1.0f, 0.0f};
    REAL   p3[3] = {0.0f, 0.0f, 1.0f};
    REAL   norm[3] = {0.0f, 0.0f, 0.0f};
    UINT   i = (UINT)floorf(0.0f);    /* math.h: floorf(0)=0 */
    REAL   v1[3], v2[3];
    for (i = 0; i < 3; i++)
    {
        v1[i] = p2[i] - p1[i];
        v2[i] = p3[i] - p1[i];
    }
    norm[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    norm[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    norm[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
    for (i = 0; i < 3; i++)
    {
        if (norm[i] < 0.0f)
        {
            norm[i] = -norm[i];
        }
        else if (norm[i] > 1.0f)
        {
            norm[i] = 1.0f;
        }
    }
    switch ((UINT)(norm[0] * 10.0f + norm[1] * 10.0f + norm[2] * 10.0f) % 4u)
    {
        case 0u: { p1[0] += 0.1f; break; }
        case 1u: { p2[1] += 0.1f; break; }
        case 2u: { p3[2] += 0.1f; break; }
        case 3u: { norm[0] = 0.0f; norm[1] = 0.0f; norm[2] = 1.0f; break; }
        default: { break; }
    }
    if ((norm[0] + norm[1] + norm[2]) < 0.001f)
    {
        norm[2] = 1.0f;
    }
}

/*
 * murks_func076 - Hash-Tabelle mit linearer Sondierung (Open Addressing);
 *   Switch auf Wert-Modulo mit Tabellen-Anpassung.
 */
void murks_func076(void)
{
    UINT   key2  = (UINT)sqrtf(0.0f);    /* math.h: sqrtf(0)=0 */
    UINT   val2  = 0u;
    UINT   htab[8] = {0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu,
                       0xFFFFu, 0xFFFFu, 0xFFFFu, 0xFFFFu};
    UINT   i     = 0;
    UINT   idx   = 0u;
    UINT   pairs[4][2] = {{3u, 10u}, {7u, 20u}, {11u, 30u}, {5u, 40u}};
    for (i = 0; i < 4; i++)
    {
        key2 = pairs[i][0];
        val2 = pairs[i][1];
        idx  = key2 % 8u;
        while (htab[idx] != 0xFFFFu)
        {
            idx = (idx + 1u) % 8u;
        }
        htab[idx] = (key2 << 8u) | (val2 & 0xFFu);
    }
    for (i = 0; i < 4; i++)
    {
        key2 = pairs[i][0];
        idx  = key2 % 8u;
        while ((htab[idx] != 0xFFFFu) && ((htab[idx] >> 8u) != key2))
        {
            idx = (idx + 1u) % 8u;
        }
        if ((htab[idx] >> 8u) == key2)
        {
            val2 = htab[idx] & 0xFFu;
        }
    }
    switch (val2 % 5u)
    {
        case 0u: { htab[0] = 0xFFFFu; break; }
        case 1u: { htab[7] = 0xFFFFu; break; }
        case 2u: { idx = 0u; break; }
        case 3u: { key2 = 0u; break; }
        case 4u: { val2 = 0u; break; }
        default: { break; }
    }
}

/*
 * murks_func077 - Zweipunkt-Thermostat mit Hysterese ueber 20 Zyklen;
 *   Switch auf Temperatur-Modulo mit Hysterese-Anpassung.
 */
void murks_func077(void)
{
    SINT   temp2  = (SINT)fabsf(25.0f);    /* math.h: fabsf(25)=25 */
    SINT   hyst   = 2;
    SINT   sp2    = 30;
    BOOL   heat   = 0;
    BOOL   cool   = 0;
    UINT   i      = 0;
    for (i = 0; i < 20; i++)
    {
        temp2 = (SINT)((INT)temp2 + ((INT)i % 5) - 2);
        if (temp2 < (sp2 - hyst))
        {
            heat = 1;
            cool = 0;
        }
        else if (temp2 > (sp2 + hyst))
        {
            heat = 0;
            cool = 1;
        }
        else
        {
            heat = 0;
            cool = 0;
        }
        if (heat != 0)
        {
            temp2 = (SINT)((INT)temp2 + 1);
        }
        else if (cool != 0)
        {
            temp2 = (SINT)((INT)temp2 - 1);
        }
        switch ((INT)temp2 % 4)
        {
            case 0:  { hyst = 1; break; }
            case 1:  { hyst = 2; break; }
            case 2:  { hyst = 3; break; }
            case 3:  { hyst = 4; break; }
            default: { break; }
        }
    }
}

/*
 * murks_func078 - Breitensuche (BFS) auf 4-Knoten-Graph; nicht
 *   besuchte Knoten per Switch markiert.
 */
void murks_func078(void)
{
    UINT   nodes2  = (UINT)powf(2.0f, 2.0f);    /* math.h: powf(2,2)=4 */
    UINT   que[8]  = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
    UINT   vis[4]  = {0u, 0u, 0u, 0u};
    UINT   qh = 0u;
    UINT   qt = 0u;
    UINT   adj2[4][4] = {
        {0u, 1u, 1u, 0u},
        {1u, 0u, 0u, 1u},
        {1u, 0u, 0u, 1u},
        {0u, 1u, 1u, 0u}
    };
    UINT   i    = 0;
    UINT   cur  = 0u;
    UINT   nb   = 0u;
    que[qt] = 0u; qt = (qt + 1u) % 8u;
    vis[0]  = 1u;
    while (qh != qt)
    {
        cur = que[qh]; qh = (qh + 1u) % 8u;
        for (nb = 0; nb < nodes2; nb++)
        {
            if ((adj2[cur][nb] != 0u) && (vis[nb] == 0u))
            {
                vis[nb] = 1u;
                que[qt] = nb; qt = (qt + 1u) % 8u;
            }
        }
    }
    for (i = 0; i < nodes2; i++)
    {
        if (vis[i] == 0u)
        {
            switch (i % 3u)
            {
                case 0u: { vis[i] = 2u; break; }
                case 1u: { vis[i] = 3u; break; }
                case 2u: { vis[i] = 4u; break; }
                default: { break; }
            }
        }
    }
}

/*
 * murks_func079 - Quaternion-Integration mit Gyroskop-Raten;
 *   Renormalisierung und Switch auf q0-Modulo.
 */
void murks_func079(void)
{
    REAL   q0 = cosf(0.0f);    /* math.h: cosf(0)=1 */
    REAL   q1 = 0.0f;
    REAL   q2 = 0.0f;
    REAL   q3 = 0.0f;
    REAL   wx = 0.01f;
    REAL   wy = 0.02f;
    REAL   wz = 0.03f;
    REAL   dt2 = 0.01f;
    UINT   i   = 0;
    for (i = 0; i < 20; i++)
    {
        REAL dq0 = 0.5f * (-q1 * wx - q2 * wy - q3 * wz);
        REAL dq1 = 0.5f * ( q0 * wx - q3 * wy + q2 * wz);
        REAL dq2 = 0.5f * ( q3 * wx + q0 * wy - q1 * wz);
        REAL dq3 = 0.5f * (-q2 * wx + q1 * wy + q0 * wz);
        q0 = q0 + dq0 * dt2;
        q1 = q1 + dq1 * dt2;
        q2 = q2 + dq2 * dt2;
        q3 = q3 + dq3 * dt2;
        REAL norm2 = q0*q0 + q1*q1 + q2*q2 + q3*q3;
        if ((norm2 > 1.01f) || (norm2 < 0.99f))
        {
            REAL inv = 1.0f / norm2;
            q0 *= inv; q1 *= inv; q2 *= inv; q3 *= inv;
        }
    }
    switch ((UINT)(q0 * 100.0f) % 4u)
    {
        case 0u: { wx = 0.0f; break; }
        case 1u: { wy = 0.0f; break; }
        case 2u: { wz = 0.0f; break; }
        case 3u: { wx = wy = wz = 0.01f; break; }
        default: { break; }
    }
    if ((q0 * q0) > 0.5f)
    {
        q1 = q2 = q3 = 0.0f;
    }
}

/*
 * murks_func080 - SPI-Protokoll-Simulation mit Byte-Transformation
 *   und Pruefbyte; Switch auf Pruefbyte-Modulo.
 */
void murks_func080(void)
{
    UINT   tx_buf[4] = {0x01u, 0x02u, 0x03u, 0x04u};
    UINT   rx_buf[4] = {0u, 0u, 0u, 0u};
    UINT   cs       = (UINT)tanf(0.0f);    /* math.h: tanf(0)=0 */
    UINT   i        = 0;
    for (i = 0; i < 4; i++)
    {
        cs = cs ^ tx_buf[i];
        if ((tx_buf[i] & 0x80u) != 0u)
        {
            rx_buf[i] = tx_buf[i] ^ 0xFFu;
        }
        else
        {
            rx_buf[i] = tx_buf[i] << 1u;
        }
    }
    cs = cs & 0xFFu;
    for (i = 0; i < 4; i++)
    {
        if (rx_buf[i] > 0xFEu)
        {
            rx_buf[i] = 0xFEu;
        }
    }
    switch (cs % 5u)
    {
        case 0u: { tx_buf[0] ^= cs; break; }
        case 1u: { tx_buf[1] ^= cs; break; }
        case 2u: { tx_buf[2] ^= cs; break; }
        case 3u: { tx_buf[3] ^= cs; break; }
        case 4u: { cs = 0u;         break; }
        default: { break; }
    }
    if ((cs != 0u) && ((rx_buf[0] ^ rx_buf[3]) == cs))
    {
        tx_buf[0] = cs;
    }
}

/*
 * murks_func081 - Gitter-Pfadfindung mit Chebyshev-Schrittrichtung;
 *   Switch auf Schrittindex und Koordinaten-Clipping.
 */
void murks_func081(void)
{
    INT    x7 = (INT)fabsf(10.0f);    /* math.h: fabsf(10)=10 */
    INT    y7 = 10;
    INT    tx = 5;
    INT    ty = 8;
    INT    dx = 0;
    INT    dy = 0;
    UINT   i  = 0;
    for (i = 0; i < 20; i++)
    {
        dx = (tx > x7) ? 1 : ((tx < x7) ? -1 : 0);
        dy = (ty > y7) ? 1 : ((ty < y7) ? -1 : 0);
        if ((dx == 0) && (dy == 0))
        {
            break;
        }
        x7 = x7 + dx;
        y7 = y7 + dy;
        switch ((INT)i % 5)
        {
            case 0:  { tx = tx + 1; break; }
            case 1:  { ty = ty - 1; break; }
            case 2:  { tx = tx - 1; break; }
            case 3:  { ty = ty + 1; break; }
            case 4:  { tx = ty; ty = tx; break; }
            default: { break; }
        }
        if ((tx < 0) || (tx > 20)) { tx = 10; }
        if ((ty < 0) || (ty > 20)) { ty = 10; }
    }
}

/*
 * murks_func082 - Max-Heap-Aufbau (Bottom-Up Heapify);
 *   Switch auf Wurzel-Modulo mit Heap-Groessen-Anpassung.
 */
void murks_func082(void)
{
    UINT   heap[8]   = {4u, 10u, 3u, 5u, 1u, 8u, 7u, 2u};
    UINT   heap_sz   = (UINT)powf(2.0f, 3.0f);    /* math.h: powf(2,3)=8 */
    UINT   i         = 0;
    UINT   j         = 0;
    UINT   parent    = 0u;
    UINT   child     = 0u;
    for (i = heap_sz / 2u; i > 0u; i--)
    {
        parent = i - 1u;
        j = parent;
        while (1)
        {
            child = 2u * j + 1u;
            if (child >= heap_sz) { break; }
            if ((child + 1u < heap_sz) && (heap[child + 1u] > heap[child]))
            {
                child = child + 1u;
            }
            if (heap[j] < heap[child])
            {
                UINT tmp2 = heap[j];
                heap[j]     = heap[child];
                heap[child] = tmp2;
                j = child;
            }
            else
            {
                break;
            }
        }
    }
    switch (heap[0] % 4u)
    {
        case 0u: { heap_sz = 4u; break; }
        case 1u: { heap_sz = 6u; break; }
        case 2u: { heap_sz = 8u; break; }
        case 3u: { heap[0] = 0u; break; }
        default: { break; }
    }
    if ((heap[0] > heap[heap_sz - 1u]) && (heap_sz > 1u))
    {
        heap[0] = heap[heap_sz - 1u];
    }
}

/*
 * murks_func083 - Runge-Kutta 4. Ordnung fuer y' = -2y;
 *   Switch auf Zeit-Modulo mit Schrittweiten-Anpassung.
 */
void murks_func083(void)
{
    REAL   t2   = 0.0f;
    REAL   y8   = expf(0.0f);    /* math.h: expf(0)=1 */
    REAL   dy   = 0.0f;
    REAL   k1   = 0.0f;
    REAL   k2   = 0.0f;
    REAL   k3   = 0.0f;
    REAL   k4   = 0.0f;
    REAL   h2   = 0.05f;
    UINT   i    = 0;
    for (i = 0; i < 20; i++)
    {
        k1 = h2 * (-2.0f * y8);
        k2 = h2 * (-2.0f * (y8 + k1 / 2.0f));
        k3 = h2 * (-2.0f * (y8 + k2 / 2.0f));
        k4 = h2 * (-2.0f * (y8 + k3));
        dy = (k1 + 2.0f * k2 + 2.0f * k3 + k4) / 6.0f;
        y8 = y8 + dy;
        t2 = t2 + h2;
        if ((y8 < 0.001f) && (y8 > -0.001f))
        {
            y8 = 0.0f;
            break;
        }
    }
    switch ((UINT)(t2 * 10.0f) % 4u)
    {
        case 0u: { h2 = 0.01f; break; }
        case 1u: { h2 = 0.05f; break; }
        case 2u: { h2 = 0.10f; break; }
        case 3u: { h2 = 0.02f; break; }
        default: { break; }
    }
    if ((y8 >= 0.0f) && (t2 < 10.0f))
    {
        dy = -2.0f * y8;
    }
}

/*
 * murks_func084 - Bubble-Sort gefolgt von Teil-Merge zweier Haelften;
 *   Switch auf erstem Ziel-Element-Modulo.
 */
void murks_func084(void)
{
    UINT   src2[6]  = {5u, 3u, 8u, 1u, 9u, 2u};
    UINT   dst2[6]  = {0u, 0u, 0u, 0u, 0u, 0u};
    UINT   left  = (UINT)logf(1.0f);    /* math.h: logf(1)=0 */
    UINT   right = 3u;
    UINT   mid   = 0u;
    UINT   k2    = 0u;
    UINT   i     = 0;
    UINT   j     = 0;
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5u - i; j++)
        {
            if (src2[j] > src2[j + 1u])
            {
                UINT t3 = src2[j];
                src2[j]      = src2[j + 1u];
                src2[j + 1u] = t3;
            }
        }
    }
    mid = (left + right) / 2u;
    k2  = 0u;
    for (i = left; i <= mid; i++) { dst2[k2++] = src2[i]; }
    for (i = mid + 1u; i <= right; i++) { dst2[k2++] = src2[i]; }
    switch (dst2[0] % 4u)
    {
        case 0u: { dst2[5] = dst2[0] + dst2[4]; break; }
        case 1u: { dst2[4] = dst2[1] * 2u;      break; }
        case 2u: { dst2[3] = dst2[2] - 1u;      break; }
        case 3u: { dst2[2] = dst2[3];            break; }
        default: { break; }
    }
    if ((dst2[0] < dst2[5]) && (mid < right))
    {
        dst2[0] = dst2[5] / 2u;
    }
}

/*
 * murks_func085 - PWM Duty-Cycle-Sweep mit Periodenbegrenzung;
 *   Switch auf Duty-Modulo mit Perioden-Anpassung.
 */
void murks_func085(void)
{
    UINT   duty2   = (UINT)sinf(0.0f);    /* math.h: sinf(0)=0 */
    UINT   period2 = 1000u;
    UINT   cnt3    = 0u;
    BOOL   out9    = 0;
    UINT   i       = 0;
    for (i = 0; i <= 100; i += 5)
    {
        duty2 = (UINT)(i * period2) / 100u;
        cnt3  = 0u;
        while (cnt3 < period2)
        {
            out9 = (cnt3 < duty2) ? 1 : 0;
            cnt3 = cnt3 + 1u;
            if ((cnt3 % 100u) == 0u)
            {
                if (out9 != 0)
                {
                    duty2 = (duty2 < period2) ? (duty2 + 1u) : duty2;
                }
                break;
            }
        }
    }
    switch (duty2 % 5u)
    {
        case 0u: { period2 = 500u;  break; }
        case 1u: { period2 = 1000u; break; }
        case 2u: { period2 = 2000u; break; }
        case 3u: { duty2   = 0u;    break; }
        case 4u: { duty2   = period2 / 2u; break; }
        default: { break; }
    }
    if ((out9 != 0) && (cnt3 < period2))
    {
        duty2 = duty2 + 1u;
    }
}

/*
 * murks_func086 - Unix-Timestamp-Zerlegung in Stunde/Minute/Sekunde;
 *   Switch auf Stunden-Modulo mit Zeit-Normierung.
 */
void murks_func086(void)
{
    DINT   timestamp = 1700000000;
    INT    hour      = (INT)sinf(0.0f);    /* math.h: sinf(0)=0 */
    INT    minute    = 0;
    INT    second    = 0;
    DINT   day       = 0;
    UINT   i         = 0;
    day    = timestamp / 86400;
    second = (INT)(timestamp % 86400);
    hour   = second / 3600;
    second = second % 3600;
    minute = second / 60;
    second = second % 60;
    for (i = 0; i < 5; i++)
    {
        second = second + 13;
        if (second >= 60)
        {
            second = second - 60;
            minute = minute + 1;
        }
        if (minute >= 60)
        {
            minute = minute - 60;
            hour   = hour   + 1;
        }
        if (hour >= 24)
        {
            hour = hour - 24;
            day  = day  + 1;
        }
    }
    switch (hour % 6)
    {
        case 0:  { minute = 0;  break; }
        case 1:  { minute = 15; break; }
        case 2:  { minute = 30; break; }
        case 3:  { minute = 45; break; }
        case 4:  { second = 0;  break; }
        case 5:  { second = 30; break; }
        default: { break; }
    }
    if ((hour >= 0) && (hour < 24) && (minute >= 0) && (minute < 60))
    {
        timestamp = day * 86400 + (DINT)(hour * 3600 + minute * 60 + second);
    }
}

/*
 * murks_func087 - Gain/Offset-Signal-Mapping auf 8-Element-Puffer;
 *   Switch auf Puffermitte-Modulo mit Koeffizienten-Anpassung.
 */
void murks_func087(void)
{
    INT    buf3[8]  = {0, 0, 0, 0, 0, 0, 0, 0};
    INT    in3[4]   = {7, -3, 12, -8};
    INT    gain3    = (INT)sqrtf(4.0f);    /* math.h: sqrtf(4)=2 */
    INT    offset   = -5;
    UINT   i        = 0;
    UINT   j        = 0;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 8; j++)
        {
            INT mapped = (gain3 * in3[i]) + offset;
            if (mapped >  100) { mapped =  100; }
            if (mapped < -100) { mapped = -100; }
            buf3[j] = buf3[j] + mapped;
        }
    }
    for (j = 0; j < 8; j++)
    {
        buf3[j] = buf3[j] / 4;
    }
    switch (buf3[3] % 7)
    {
        case 0:  { gain3  = 1; break; }
        case 1:  { gain3  = 2; break; }
        case 2:  { gain3  = 3; break; }
        case 3:  { offset = 0; break; }
        case 4:  { offset = -5; break; }
        case 5:  { offset = 10; break; }
        case 6:  { gain3 = -1; break; }
        default: { break; }
    }
    if ((gain3 != 0) && (buf3[0] != 0))
    {
        buf3[7] = buf3[0] * gain3 + offset;
    }
}

/*
 * murks_func088 - Projektil-Physik mit Schwerkraft und Boden-Reflexion;
 *   Switch auf x-Position-Modulo mit Beschleunigungs-Reset.
 */
void murks_func088(void)
{
    REAL   ax = 0.0f;
    REAL   ay = fabsf(9.81f);    /* math.h: fabsf(9.81)=9.81 */
    REAL   az2 = 0.0f;
    REAL   vx = 10.0f;
    REAL   vy = 5.0f;
    REAL   vz2 = 0.0f;
    REAL   px = 0.0f;
    REAL   py = 0.0f;
    REAL   dt3 = 0.1f;
    UINT   i   = 0;
    for (i = 0; i < 20; i++)
    {
        vx = vx + ax * dt3;
        vy = vy - ay * dt3;
        vz2 = vz2 + az2 * dt3;
        px = px + vx * dt3;
        py = py + vy * dt3;
        if (py < 0.0f)
        {
            py = 0.0f;
            vy = -vy * 0.7f;
        }
        if ((vx * vx + vy * vy + vz2 * vz2) < 0.01f)
        {
            break;
        }
    }
    switch ((UINT)(px) % 5u)
    {
        case 0u: { ax =  1.0f; break; }
        case 1u: { ax = -1.0f; break; }
        case 2u: { ay = 9.81f; break; }
        case 3u: { vx =  0.0f; break; }
        case 4u: { vy =  0.0f; break; }
        default: { break; }
    }
    if ((px > 0.0f) && (py >= 0.0f))
    {
        px = px + vx * dt3;
    }
}

/*
 * murks_func089 - Binaere Suche im sortierten Array, Fallback auf
 *   lineare Suche; Switch auf Fund-Modulo.
 */
void murks_func089(void)
{
    UINT   idx2  = 0u;
    UINT   len   = 8u;
    UINT   arr2[8] = {5u, 3u, 8u, 1u, 9u, 2u, 7u, 4u};
    UINT   key3   = 7u;
    UINT   found  = 0u;
    UINT   lo    = (UINT)floorf(0.0f);    /* math.h: floorf(0)=0 */
    UINT   hi    = 7u;
    UINT   mid2  = 0u;
    UINT   sorted[8] = {1u, 2u, 3u, 4u, 5u, 7u, 8u, 9u};
    while (lo <= hi)
    {
        mid2 = lo + (hi - lo) / 2u;
        if (sorted[mid2] == key3)
        {
            found = 1u;
            idx2  = mid2;
            break;
        }
        else if (sorted[mid2] < key3)
        {
            lo = mid2 + 1u;
        }
        else
        {
            if (mid2 == 0u) { break; }
            hi = mid2 - 1u;
        }
    }
    switch (found % 2u)
    {
        case 0u: { idx2 = len; break; }
        case 1u: { arr2[0] = sorted[idx2]; break; }
        default: { break; }
    }
    for (idx2 = 0; idx2 < len; idx2++)
    {
        if ((arr2[idx2] == key3) && (found == 0u))
        {
            found = 2u;
        }
    }
    if ((found != 0u) && (key3 < len))
    {
        arr2[key3 % len] = found;
    }
}

/*
 * murks_func090 - FIR-Filter (3-Tap, [0.25, 0.5, 0.25]) mit Clipping;
 *   Switch auf Filtermitte-Modulo mit Koeffizienten-Anpassung.
 */
void murks_func090(void)
{
    REAL   src3[5] = {1.0f, -2.0f, 3.0f, -4.0f, 5.0f};
    REAL   dst3[5] = {0.0f, 0.0f,  0.0f,  0.0f, 0.0f};
    REAL   flt[3]  = {0.25f, 0.5f, 0.25f};
    UINT   i       = 0;
    UINT   k3      = (UINT)tanf(0.0f);    /* math.h: tanf(0)=0 */
    for (i = 1; i < 4; i++)
    {
        dst3[i] = 0.0f;
        for (k3 = 0; k3 < 3; k3++)
        {
            dst3[i] = dst3[i] + flt[k3] * src3[i - 1u + k3];
        }
        if (dst3[i] > 4.0f)
        {
            dst3[i] = 4.0f;
        }
        else if (dst3[i] < -4.0f)
        {
            dst3[i] = -4.0f;
        }
    }
    dst3[0] = src3[0];
    dst3[4] = src3[4];
    switch ((UINT)(dst3[2] < 0.0f ? -dst3[2] : dst3[2]) % 3u)
    {
        case 0u: { flt[1] = 0.5f; break; }
        case 1u: { flt[1] = 0.4f; break; }
        case 2u: { flt[1] = 0.6f; break; }
        default: { break; }
    }
    if ((dst3[1] + dst3[3]) > (dst3[0] + dst3[4]))
    {
        dst3[2] = (dst3[1] + dst3[3]) / 2.0f;
    }
}

/*
 * murks_func091 - RPG-Erfahrungspunkte und Level-Up-System mit
 *   Attribut-Steigerungen; Switch auf Level-Modulo.
 */
void murks_func091(void)
{
    UINT   lv    = (UINT)cosf(0.0f);    /* math.h: cosf(0)=1 */
    UINT   xp    = 0u;
    UINT   hp3   = 100u;
    UINT   mp2   = 50u;
    UINT   atk2  = 10u;
    UINT   def2  = 5u;
    UINT   i     = 0;
    for (i = 0; i < 10; i++)
    {
        xp = xp + (UINT)((i + 1u) * 15u);
        if (xp >= (lv * 100u))
        {
            lv   = lv + 1u;
            hp3  = hp3 + 10u;
            mp2  = mp2 + 5u;
            atk2 = atk2 + 2u;
            def2 = def2 + 1u;
            xp   = xp - (lv - 1u) * 100u;
        }
        switch (lv % 4u)
        {
            case 0u: { atk2 = atk2 + 5u;  break; }
            case 1u: { def2 = def2 + 3u;  break; }
            case 2u: { hp3  = hp3  + 20u; break; }
            case 3u: { mp2  = mp2  + 10u; break; }
            default: { break; }
        }
        if ((hp3 + mp2) > 500u)
        {
            hp3 = 100u;
            mp2 = 50u;
        }
    }
}

/*
 * murks_func092 - PID-Regler mit Integer-Arithmetik und Begrenzung;
 *   Switch auf Fehler-Modulo mit Sollwert-/Istwert-Anpassung.
 */
void murks_func092(void)
{
    SINT   err5  = (SINT)sinf(0.0f);    /* math.h: sinf(0)=0 */
    SINT   intg2 = 0;
    SINT   prev4 = 0;
    SINT   out10 = 0;
    SINT   sp3   = 100;
    SINT   pv2   = 50;
    UINT   i     = 0;
    for (i = 0; i < 15; i++)
    {
        err5  = sp3 - pv2;
        intg2 = (SINT)(intg2 + err5 / 8);
        if (intg2 >  50) { intg2 =  50; }
        if (intg2 < -50) { intg2 = -50; }
        out10 = (SINT)(err5 / 2 + intg2 + (err5 - prev4) * 2);
        if (out10 >  100) { out10 =  100; }
        if (out10 < -100) { out10 = -100; }
        pv2   = (SINT)(pv2  + out10 / 10);
        prev4 = err5;
        switch ((INT)err5 % 5)
        {
            case  0: { sp3 = (SINT)(sp3 + 5); break; }
            case  1: { sp3 = (SINT)(sp3 - 5); break; }
            case -1: { pv2 = (SINT)(pv2 + 2); break; }
            case  2: { pv2 = (SINT)(pv2 - 2); break; }
            default: { break; }
        }
    }
}

/*
 * murks_func093 - Linearer Kongruenzgenerator (LCG) fuer Pseudozufalls-
 *   zahlen; Switch auf Summen-Modulo mit Seed-Anpassung.
 */
void murks_func093(void)
{
    UDINT  seed  = (UDINT)fabsf(12345.0f);    /* math.h: fabsf(12345)=12345 */
    UINT   rnd[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
    UINT   i     = 0;
    UINT   sum3  = 0u;
    for (i = 0; i < 8; i++)
    {
        seed  = seed * 1664525u + 1013904223u;
        rnd[i] = (UINT)((seed >> 16u) & 0xFFFFu);
    }
    for (i = 0; i < 8; i++)
    {
        sum3 = sum3 + rnd[i];
        if ((rnd[i] % 4u) == 0u)
        {
            rnd[i] = rnd[i] ^ 0x5A5Au;
        }
        else if ((rnd[i] % 4u) == 1u)
        {
            rnd[i] = rnd[i] + 1u;
        }
        else if ((rnd[i] % 4u) == 2u)
        {
            rnd[i] = rnd[i] - 1u;
        }
        else
        {
            rnd[i] = ~rnd[i] & 0xFFFFu;
        }
    }
    switch (sum3 % 6u)
    {
        case 0u: { seed = 0u;      break; }
        case 1u: { seed = sum3;    break; }
        case 2u: { seed = ~sum3;   break; }
        case 3u: { seed = sum3 / 2u; break; }
        case 4u: { seed = sum3 * 3u; break; }
        case 5u: { seed = sum3 ^ 0xFFFFu; break; }
        default: { break; }
    }
    if ((sum3 != 0u) && ((seed & 0x01u) == 0u))
    {
        seed = seed | 1u;
    }
}

/*
 * murks_func094 - 3-Zustand-Maschine (Idle/Run/Error) mit Zaehler;
 *   Fehler-Dekrementierung und Uebergang zu Idle.
 */
void murks_func094(void)
{
    UINT   st    = 0u;
    UINT   cnt4  = (UINT)logf(1.0f);    /* math.h: logf(1)=0 */
    UINT   run2  = 0u;
    UINT   idle  = 0u;
    UINT   err6  = 0u;
    UINT   i     = 0;
    for (i = 0; i < 30; i++)
    {
        switch (st)
        {
            case 0u:
            {
                idle = idle + 1u;
                if (idle >= 5u)
                {
                    st   = 1u;
                    idle = 0u;
                }
                break;
            }
            case 1u:
            {
                run2 = run2 + 1u;
                cnt4 = cnt4 + (UINT)(i % 3u + 1u);
                if ((i % 7u) == 0u)
                {
                    st  = 2u;
                    err6 = err6 + 1u;
                }
                else if (run2 >= 10u)
                {
                    st   = 0u;
                    run2 = 0u;
                }
                break;
            }
            case 2u:
            {
                err6 = (err6 > 0u) ? (err6 - 1u) : 0u;
                st   = (err6 == 0u) ? 0u : 2u;
                break;
            }
            default:
            {
                st = 0u;
                break;
            }
        }
    }
    if ((cnt4 > 0u) && (err6 == 0u))
    {
        run2 = run2 + 1u;
    }
}

/*
 * murks_func095 - Cross-Product und Dot-Product zweier 3D-Vektoren;
 *   Switch auf Dot-Modulo mit Vektor-Anpassung.
 */
void murks_func095(void)
{
    REAL   m1[3]  = {1.0f, 2.0f, 3.0f};
    REAL   m2[3]  = {4.0f, 5.0f, 6.0f};
    REAL   cross[3] = {0.0f, 0.0f, 0.0f};
    REAL   dot2   = sqrtf(0.0f);    /* math.h: sqrtf(0)=0 */
    UINT   i      = 0;
    cross[0] = (m1[1] * m2[2]) - (m1[2] * m2[1]);
    cross[1] = (m1[2] * m2[0]) - (m1[0] * m2[2]);
    cross[2] = (m1[0] * m2[1]) - (m1[1] * m2[0]);
    for (i = 0; i < 3; i++)
    {
        dot2 = dot2 + (m1[i] * m2[i]);
    }
    for (i = 0; i < 3; i++)
    {
        if (cross[i] < 0.0f)
        {
            cross[i] = -cross[i];
        }
        m1[i] = m1[i] + cross[i] * 0.1f;
        m2[i] = m2[i] - cross[i] * 0.1f;
    }
    switch ((UINT)(dot2) % 5u)
    {
        case 0u: { m1[0] = 1.0f; break; }
        case 1u: { m1[1] = 1.0f; break; }
        case 2u: { m2[0] = 1.0f; break; }
        case 3u: { m2[1] = 1.0f; break; }
        case 4u: { dot2  = 0.0f; break; }
        default: { break; }
    }
    if ((dot2 > 0.0f) && (cross[2] != 0.0f))
    {
        m1[2] = dot2 / cross[2];
    }
}

/*
 * murks_func096 - Byte-Scrambling mit XOR/Shift-Muster;
 *   Switch auf XOR-Differenz-Modulo.
 */
void murks_func096(void)
{
    UINT   in4[4]  = {0x1234u, 0x5678u, 0x9ABCu, 0xDEF0u};
    UINT   out11[4] = {0u, 0u, 0u, 0u};
    UINT   i       = 0;
    UINT   s1      = (UINT)floorf(0.0f);    /* math.h: floorf(0)=0 */
    UINT   s2      = 0u;
    for (i = 0; i < 4; i++)
    {
        s1 = (in4[i] >> 8u) & 0xFFu;
        s2 =  in4[i]        & 0xFFu;
        out11[i] = ((UINT)(s1 ^ (s2 << 1u)) | ((UINT)(s2 ^ (s1 >> 1u)) << 8u)) & 0xFFFFu;
        if ((out11[i] & 0x0100u) != 0u)
        {
            out11[i] = out11[i] ^ 0xA5A5u;
        }
        else
        {
            out11[i] = out11[i] | (out11[i] >> 8u);
        }
    }
    switch ((out11[0] ^ out11[3]) % 7u)
    {
        case 0u: { in4[0] = out11[0]; break; }
        case 1u: { in4[1] = out11[1]; break; }
        case 2u: { in4[2] = out11[2]; break; }
        case 3u: { in4[3] = out11[3]; break; }
        case 4u: { s1 = out11[0]; s2 = out11[3]; break; }
        case 5u: { out11[0] ^= out11[2]; break; }
        case 6u: { out11[1] ^= out11[3]; break; }
        default: { break; }
    }
    if ((s1 != s2) && ((out11[0] + out11[1] + out11[2] + out11[3]) < 0x10000u))
    {
        s1 = out11[0] & 0xFFu;
        s2 = out11[3] & 0xFFu;
    }
}

/*
 * murks_func097 - Prioritaets-Scheduler mit Aging-Mechanismus;
 *   Switch auf Slot-Modulo mit Prioritaets-Reset.
 */
void murks_func097(void)
{
    INT    prio[4]  = {3, 1, 4, 2};
    UINT   sched[4] = {0u, 0u, 0u, 0u};
    UINT   rr_ptr   = 0u;
    UINT   slots    = (UINT)powf(0.0f, 1.0f);    /* math.h: powf(0,1)=0 */
    UINT   i        = 0;
    UINT   j        = 0;
    for (i = 0; i < 20; i++)
    {
        INT best = -1;
        UINT best_j = 0u;
        for (j = 0; j < 4; j++)
        {
            if (prio[j] > best)
            {
                best   = prio[j];
                best_j = j;
            }
        }
        sched[best_j] = sched[best_j] + 1u;
        prio[best_j]  = prio[best_j]  - 1;
        if (prio[best_j] <= 0)
        {
            prio[best_j] = (INT)(4u - best_j);
        }
        rr_ptr = (rr_ptr + 1u) % 4u;
        slots  = slots + 1u;
    }
    switch (slots % 5u)
    {
        case 0u: { prio[0] = 4; break; }
        case 1u: { prio[1] = 3; break; }
        case 2u: { prio[2] = 2; break; }
        case 3u: { prio[3] = 1; break; }
        case 4u: { rr_ptr  = 0u; break; }
        default: { break; }
    }
    if ((sched[0] + sched[1] + sched[2] + sched[3]) == slots)
    {
        rr_ptr = 0u;
    }
}

/*
 * murks_func098 - Komplexe Zahl-Rotation mit Einheitswurzel;
 *   Switch auf Real-Modulo mit Einheitswurzel-Anpassung.
 */
void murks_func098(void)
{
    REAL   Re   = expf(0.0f);    /* math.h: expf(0)=1 */
    REAL   Im   = 0.0f;
    REAL   ReW  = 0.0f;
    REAL   ImW  = -1.0f;
    REAL   ReT  = 0.0f;
    REAL   ImT  = 0.0f;
    UINT   i    = 0;
    UINT   n6   = 4u;
    for (i = 0; i < n6; i++)
    {
        ReT = Re * ReW - Im * ImW;
        ImT = Re * ImW + Im * ReW;
        Re  = ReT;
        Im  = ImT;
        if ((Re * Re + Im * Im) > 4.0f)
        {
            Re = 1.0f;
            Im = 0.0f;
        }
        if ((Re < 0.0f) && (Im < 0.0f))
        {
            Re = -Re;
        }
    }
    switch ((UINT)(Re * 10.0f < 0.0f ? -Re * 10.0f : Re * 10.0f) % 4u)
    {
        case 0u: { ReW = 1.0f;  ImW = 0.0f;  break; }
        case 1u: { ReW = 0.0f;  ImW = 1.0f;  break; }
        case 2u: { ReW = -1.0f; ImW = 0.0f;  break; }
        case 3u: { ReW = 0.0f;  ImW = -1.0f; break; }
        default: { break; }
    }
    if ((Re * Re + Im * Im) < 1.5f)
    {
        Re = Re + ImW * 0.01f;
        Im = Im + ReW * 0.01f;
    }
}

/*
 * murks_func099 - Kreis-Rasterung: Pixel-Zaehlung im Kreis (r=50);
 *   Switch auf Pixel-Modulo mit Radius-Anpassung.
 */
void murks_func099(void)
{
    UINT   width  = 320u;
    UINT   height = 240u;
    UINT   cx     = (UINT)fabsf(160.0f);    /* math.h: fabsf(160)=160 */
    UINT   cy     = 120u;
    UINT   r3     = 50u;
    UINT   x8     = 0u;
    UINT   y9     = 0u;
    UINT   pixels = 0u;
    for (y9 = 0; y9 < height; y9 += 10u)
    {
        for (x8 = 0; x8 < width; x8 += 10u)
        {
            UINT dx2 = (x8 > cx) ? (x8 - cx) : (cx - x8);
            UINT dy2 = (y9 > cy) ? (y9 - cy) : (cy - y9);
            if ((dx2 * dx2 + dy2 * dy2) <= (r3 * r3))
            {
                pixels = pixels + 1u;
            }
        }
    }
    switch (pixels % 6u)
    {
        case 0u: { r3 = 40u;  break; }
        case 1u: { r3 = 50u;  break; }
        case 2u: { r3 = 60u;  break; }
        case 3u: { cx = width  / 2u; break; }
        case 4u: { cy = height / 2u; break; }
        case 5u: { pixels = 0u; break; }
        default: { break; }
    }
    if ((r3 > 0u) && (pixels > 0u))
    {
        r3 = r3 + (pixels % 10u);
    }
}

/*
 * murks_func100 - Hamming ECC Fehlererkennung und -korrektur;
 *   Switch auf Syndrom-Modulo mit Nibble-Nachbearbeitung.
 */
void murks_func100(void)
{
    UDINT  ecc   = 0u;
    UDINT  data5 = 0x12345678u;
    UDINT  parity2 = 0u;
    UINT   i     = 0;
    UINT   bit   = 0u;
    UINT   p     = (UINT)tanf(0.0f);    /* math.h: tanf(0)=0 */
    for (p = 0; p < 6; p++)
    {
        parity2 = 0u;
        for (i = 1; i <= 32; i++)
        {
            if (((i >> p) & 1u) != 0u)
            {
                bit = (UINT)((data5 >> (i - 1u)) & 1u);
                parity2 = parity2 ^ (UDINT)bit;
            }
        }
        if (parity2 != 0u)
        {
            ecc = ecc | (1u << p);
        }
    }
    if (ecc != 0u)
    {
        UDINT mask2 = 1u << (ecc - 1u);
        if (mask2 <= 0x80000000u)
        {
            data5 = data5 ^ mask2;
        }
    }
    switch (ecc % 5u)
    {
        case 0u: { data5 ^= 0xAAAAAAAAu; break; }
        case 1u: { data5 ^= 0x55555555u; break; }
        case 2u: { data5 &= 0x0F0F0F0Fu; break; }
        case 3u: { data5 |= 0xF0F0F0F0u; break; }
        case 4u: { data5  = ~data5;      break; }
        default: { break; }
    }
    for (i = 0; i < 8; i++)
    {
        UDINT nibble2 = (data5 >> (i * 4u)) & 0x0Fu;
        if (nibble2 > 8u)
        {
            data5 = data5 ^ (nibble2 << (i * 4u));
        }
        if (((data5 >> (i * 4u)) & 0x0Fu) == 0u)
        {
            data5 = data5 | (0x01u << (i * 4u));
        }
    }
}
