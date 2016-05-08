package gnu.crypto.sig;

// ----------------------------------------------------------------------------
// $Id: ISignatureCodec.java,v 1.1.1.1 2007/05/07 23:32:39 jungo Exp $
//
// Copyright (C) 2001, 2002, Free Software Foundation, Inc.
//
// This file is part of GNU Crypto.
//
// GNU Crypto is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// GNU Crypto is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; see the file COPYING.  If not, write to the
//
//    Free Software Foundation Inc.,
//    51 Franklin Street, Fifth Floor,
//    Boston, MA 02110-1301
//    USA
//
// Linking this library statically or dynamically with other modules is
// making a combined work based on this library.  Thus, the terms and
// conditions of the GNU General Public License cover the whole
// combination.
//
// As a special exception, the copyright holders of this library give
// you permission to link this library with independent modules to
// produce an executable, regardless of the license terms of these
// independent modules, and to copy and distribute the resulting
// executable under terms of your choice, provided that you also meet,
// for each linked independent module, the terms and conditions of the
// license of that module.  An independent module is a module which is
// not derived from or based on this library.  If you modify this
// library, you may extend this exception to your version of the
// library, but you are not obligated to do so.  If you do not wish to
// do so, delete this exception statement from your version.
// ----------------------------------------------------------------------------

import gnu.crypto.Registry;

/**
 * <p>The visible methods of an object that knows how to encode and decode
 * cryptographic signatures. Codecs are useful for (a) externalising signature
 * output data for storage and on-the-wire transmission, as well as (b) re-
 * creating their internal Java representation from external sources.</p>
 *
 * @version $Revision: 1.1.1.1 $
 */
public interface ISignatureCodec {

   // Constants
   // -------------------------------------------------------------------------

   /** Constant identifying the <i>Raw</i> encoding format. */
   int RAW_FORMAT = Registry.RAW_ENCODING_ID;

   // Method(s)
   // -------------------------------------------------------------------------

   int getFormatID();

   byte[] encodeSignature(Object signature);

   Object decodeSignature(byte[] input);
}
