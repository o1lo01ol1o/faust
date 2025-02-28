/**************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 
 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ***************************************************************************/

#ifndef LIBFAUSTCOMMON_C_H
#define LIBFAUSTCOMMON_C_H

#include "faust/export.h"

/*!
 \addtogroup signalc C interface for the Signal API.
 @{
 */

/**
 * Opaque types.
 */

#ifdef _MSC_VER
typedef void CTree;
#else
typedef struct {} CTree;
#endif

typedef CTree* Signal;
typedef CTree* Box;

enum SType { kSInt, kSReal };

enum SOperator { kAdd, kSub, kMul, kDiv, kRem, kLsh, kARsh, kLRsh, kGT, kLT, kGE, kLE, kEQ, kNE, kAND, kOR, kXOR };

#endif

#ifndef LIBFAUSTSIGNAL_C_H
#define LIBFAUSTSIGNAL_C_H

#ifdef __cplusplus
extern "C"
{
#endif
    
    /**
     * Create global compilation context, has to be done first.
     */
    LIBFAUST_API void createLibContext();
    
    /**
     * Destroy global compilation context, has to be done last.
     */
    LIBFAUST_API void destroyLibContext();
    
    /**
     * Constant integer : for all t, x(t) = n.
     *
     * @param n - the integer
     *
     * @return the integer value.
     */
    LIBFAUST_API Signal CsigInt(int n);
    
    /**
     * Constant real : for all t, x(t) = n.
     *
     * @param n - the float/double value (depends of -single or -double compilation parameter)
     *
     * @return the float/double signal.
     */
    LIBFAUST_API Signal CsigReal(double n);
    
    /**
     * Create an input.
     *
     * @param idx - the input index
     *
     * @return the input signal.
     */
    LIBFAUST_API Signal CsigInput(int idx);
    
    /**
     * Create a delayed signal.
     *
     * @param s - the signal to be delayed
     * @param del - the delay signal that doesn't have to be fixed but must be bounded and cannot be negative
     *
     * @return the delayed signal.
     */
    LIBFAUST_API Signal CsigDelay(Signal s, Signal del);
    
    /**
     * Create a casted signal.
     *
     * @param s - the signal to be casted in integer
     *
     * @return the casted signal.
     */
    LIBFAUST_API Signal CsigIntCast(Signal s);
    
    /**
     * Create a casted signal.
     *
     * @param s - the signal to be casted as float/double value (depends of -single or -double compilation parameter)
     *
     * @return the casted signal.
     */
    LIBFAUST_API Signal CsigFloatCast(Signal s);
    
    /**
     * Create a read only table.
     *
     * @param n - the table size, a constant numerical expression (see [1])
     * @param init - the table content
     * @param ridx - the read index (an int between 0 and n-1)
     *
     * @return the table signal.
     */
    LIBFAUST_API Signal CsigReadOnlyTable(Signal n, Signal init, Signal ridx);
    
    /**
     * Create a read/write table.
     *
     * @param n - the table size, a constant numerical expression (see [1])
     * @param init - the table content
     * @param widx - the write index (an integer between 0 and n-1)
     * @param wsig - the input of the table
     * @param ridx - the read index (an integer between 0 and n-1)
     *
     * @return the table signal.
     */
    LIBFAUST_API Signal CsigWriteReadTable(Signal n, Signal init, Signal widx, Signal wsig, Signal ridx);
    
    /**
     * Create a waveform.
     *
     * @param wf - the content of the waveform as a null terminated array of CsigInt or CsigDouble signals
     *
     * @return the waveform signal.
     */
    LIBFAUST_API Signal CsigWaveform(Signal* wf);
    // Use: CsigInt(size of wf); to generate the waveform size signal
    
    /**
     * Create a soundfile block.
     *
     * @param label - of form "label[url:{'path1';'path2';'path3'}]" to describe a list of soundfiles
     *
     * @return the soundfile block.
     */
    LIBFAUST_API Signal CsigSoundfile(const char* label);
    
    /**
     * Create the length signal of a given soundfile in frames.
     *
     * @param sf - the soundfile
     * @param part - in the [0..255] range to select a given sound number, a constant numerical expression (see [1])
     *
     * @return the length signal.
     */
    LIBFAUST_API Signal CsigSoundfileLength(Signal sf, Signal part);
    
    /**
     * Create the rate signal of a given soundfile in Hz.
     *
     * @param sf - the soundfile
     * @param part - in the [0..255] range to select a given sound number, a constant numerical expression (see [1])
     *
     * @return the rate signal.
     */
    LIBFAUST_API Signal CsigSoundfileRate(Signal sf, Signal part);
    
    /**
     * Create the buffer signal of a given soundfile.
     *
     * @param sf - the soundfile
     * @param chan - an integer to select a given channel, a constant numerical expression (see [1])
     * @param part - in the [0..255] range to select a given sound number, a constant numerical expression (see [1])
     * @param ridx - the read index (an integer between 0 and the selected sound length)
     *
     * @return the buffer signal.
     */
    LIBFAUST_API Signal CsigSoundfileBuffer(Signal sf, Signal chan, Signal part, Signal ridx);
    
    /**
     * Create a selector between two signals.
     *
     * @param selector - when 0 at time t returns s1[t], otherwise returns s2[t]
     * (selector is automatically wrapped with sigIntCast)
     * @param s1 - first signal to be selected
     * @param s2 - second signal to be selected
     *
     * @return the selected signal depending of the selector value at each time t.
     */
    LIBFAUST_API Signal CsigSelect2(Signal selector, Signal s1, Signal s2);
    
    /**
     * Create a selector between three signals.
     *
     * @param selector - when 0 at time t returns s1[t], when 1 at time t returns s2[t], otherwise returns s3[t]
     * (selector is automatically wrapped with sigIntCast)
     * @param s1 - first signal to be selected
     * @param s2 - second signal to be selected
     * @param s3 - third signal to be selected
     *
     * @return the selected signal depending of the selector value at each time t.
     */
    LIBFAUST_API Signal CsigSelect3(Signal selector, Signal s1, Signal s2, Signal s3);
      
    /**
     * Create a foreign constant signal.
     *
     * @param type - the foreign constant type of SType
     * @param name - the foreign constant name
     * @param file - the include file where the foreign constant is defined
     *
     * @return the foreign constant signal.
     */
    LIBFAUST_API Signal CsigFConst(enum SType type, const char* name, const char* file);
    
    /**
     * Create a foreign variable signal.
     *
     * @param type - the foreign variable type of SType
     * @param name - the foreign variable name
     * @param file - the include file where the foreign variable is defined
     *
     * @return the foreign variable signal.
     */
    LIBFAUST_API Signal CsigFVar(enum SType type, const char* name, const char* file);
    
    /**
     * Generic binary mathematical functions.
     *
     * @param op - the operator in SOperator set
     * @param x - first signal
     * @param y - second signal
     *
     * @return the result signal of op(x,y).
     */
    LIBFAUST_API Signal CsigBinOp(enum SOperator op, Signal x, Signal y);
    
    /**
     * Specific binary mathematical functions.
     *
     * @param x - first signal
     * @param y - second signal
     *
     * @return the result signal of fun(x,y).
     */
    LIBFAUST_API Signal CsigAdd(Signal x, Signal y);
    LIBFAUST_API Signal CsigSub(Signal x, Signal y);
    LIBFAUST_API Signal CsigMul(Signal x, Signal y);
    LIBFAUST_API Signal CsigDiv(Signal x, Signal y);
    LIBFAUST_API Signal CsigRem(Signal x, Signal y);
    
    LIBFAUST_API Signal CsigLeftShift(Signal x, Signal y);
    LIBFAUST_API Signal CsigLRightShift(Signal x, Signal y);
    LIBFAUST_API Signal CsigARightShift(Signal x, Signal y);
    
    LIBFAUST_API Signal CsigGT(Signal x, Signal y);
    LIBFAUST_API Signal CsigLT(Signal x, Signal y);
    LIBFAUST_API Signal CsigGE(Signal x, Signal y);
    LIBFAUST_API Signal CsigLE(Signal x, Signal y);
    LIBFAUST_API Signal CsigEQ(Signal x, Signal y);
    LIBFAUST_API Signal CsigNE(Signal x, Signal y);
    
    LIBFAUST_API Signal CsigAND(Signal x, Signal y);
    LIBFAUST_API Signal CsigOR(Signal x, Signal y);
    LIBFAUST_API Signal CsigXOR(Signal x, Signal y);
    
    /**
     * Extended unary mathematical functions.
     */
    LIBFAUST_API Signal CsigAbs(Signal x);
    LIBFAUST_API Signal CsigAcos(Signal x);
    LIBFAUST_API Signal CsigTan(Signal x);
    LIBFAUST_API Signal CsigSqrt(Signal x);
    LIBFAUST_API Signal CsigSin(Signal x);
    LIBFAUST_API Signal CsigRint(Signal x);
    LIBFAUST_API Signal CsigLog(Signal x);
    LIBFAUST_API Signal CsigLog10(Signal x);
    LIBFAUST_API Signal CsigFloor(Signal x);
    LIBFAUST_API Signal CsigExp(Signal x);
    LIBFAUST_API Signal CsigExp10(Signal x);
    LIBFAUST_API Signal CsigCos(Signal x);
    LIBFAUST_API Signal CsigCeil(Signal x);
    LIBFAUST_API Signal CsigAtan(Signal x);
    LIBFAUST_API Signal CsigAsin(Signal x);
    
    /**
     * Extended binary mathematical functions.
     */
    LIBFAUST_API Signal CsigRemainder(Signal x, Signal y);
    LIBFAUST_API Signal CsigPow(Signal x, Signal y);
    LIBFAUST_API Signal CsigMin(Signal x, Signal y);
    LIBFAUST_API Signal CsigMax(Signal x, Signal y);
    LIBFAUST_API Signal CsigFmod(Signal x, Signal y);
    LIBFAUST_API Signal CsigAtan2(Signal x, Signal y);
    
    /**
     * Create a recursive signal inside the sigRecursion expression.
     *
     * @return the recursive signal.
     */
    LIBFAUST_API Signal CsigSelf();
    
    /**
     * Create a recursive signal. Use sigSelf() to refer to the
     * recursive signal inside the sigRecursion expression.
     *
     * @param s - the signal to recurse on.
     *
     * @return the signal with a recursion.
     */
    LIBFAUST_API Signal CsigRecursion(Signal s);
    
    /**
     * Create a button signal.
     *
     * @param label - the label definition (see [2])
     *
     * @return the button signal.
     */
    LIBFAUST_API Signal CsigButton(const char* label);
    
    /**
     * Create a checkbox signal.
     *
     * @param label - the label definition (see [2])
     *
     * @return the checkbox signal.
     */
    LIBFAUST_API Signal CsigCheckbox(const char* label);
    
    /**
     * Create a vertical slider signal.
     *
     * @param label - the label definition (see [2])
     * @param init - the init signal, a constant numerical expression (see [1])
     * @param min - the min signal, a constant numerical expression (see [1])
     * @param max - the max signal, a constant numerical expression (see [1])
     * @param step - the step signal, a constant numerical expression (see [1])
     *
     * @return the vertical slider signal.
     */
    LIBFAUST_API Signal CsigVSlider(const char* label, Signal init, Signal min, Signal max, Signal step);
    
    /**
     * Create an horizontal slider signal.
     *
     * @param label - the label definition (see [2])
     * @param init - the init signal, a constant numerical expression (see [1])
     * @param min - the min signal, a constant numerical expression (see [1])
     * @param max - the max signal, a constant numerical expression (see [1])
     * @param step - the step signal, a constant numerical expression (see [1])
     *
     * @return the horizontal slider signal.
     */
    LIBFAUST_API Signal CsigHSlider(const char* label, Signal init, Signal min, Signal max, Signal step);
    
    /**
     * Create a num entry signal.
     *
     * @param label - the label definition (see [2])
     * @param init - the init signal, a constant numerical expression (see [1])
     * @param min - the min signal, a constant numerical expression (see [1])
     * @param max - the max signal, a constant numerical expression (see [1])
     * @param step - the step signal, a constant numerical expression (see [1])
     *
     * @return the num entry signal.
     */
    LIBFAUST_API Signal CsigNumEntry(const char* label, Signal init, Signal min, Signal max, Signal step);
    
    /**
     * Create a vertical bargraph signal.
     *
     * @param label - the label definition (see [2])
     * @param min - the min signal, a constant numerical expression (see [1])
     * @param max - the max signal, a constant numerical expression (see [1])
     * @param s - the input signal
     *
     * @return the vertical bargraph signal.
     */
    LIBFAUST_API Signal CsigVBargraph(const char* label, Signal min, Signal max, Signal s);
    
    /**
     * Create an horizontal bargraph signal.
     *
     * @param label - the label definition (see [2])
     * @param min - the min signal, a constant numerical expression (see [1])
     * @param max - the max signal, a constant numerical expression (see [1])
     * @param s - the input signal
     *
     * @return the horizontal bargraph signal.
     */
    LIBFAUST_API Signal CsigHBargraph(const char* label, Signal min, Signal max, Signal s);
    
    /**
     * Create an attach signal.
     *
     * The attach primitive takes two input signals and produces one output signal
     * which is a copy of the first input. The role of attach is to force
     * its second input signal to be compiled with the first one.
     *
     * @param s1 - the first signal
     * @param s2 - the second signal
     *
     * @return the attach signal.
     */
    LIBFAUST_API Signal CsigAttach(Signal s1, Signal s2);
    
    /*
     [1] Constant numerical expression : see https://faustdoc.grame.fr/manual/syntax/#constant-numerical-expressions
     [2] Label definition : https://faustdoc.grame.fr/manual/syntax/#variable-parts-of-a-label
     */
    
#ifdef __cplusplus
}
#endif

/*!
 @}
 */

#endif
