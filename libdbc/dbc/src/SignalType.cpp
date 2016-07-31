/*
 * Copyright (C) 2013 Tobias Lorenz.
 * Contact: tobias.lorenz@gmx.net
 *
 * This file is part of Tobias Lorenz's Toolkit.
 *
 * Commercial License Usage
 * Licensees holding valid commercial licenses may use this file in
 * accordance with the commercial license agreement provided with the
 * Software or, alternatively, in accordance with the terms contained in
 * a written agreement between you and Tobias Lorenz.
 *
 * GNU General Public License 3.0 Usage
 * Alternatively, this file may be used under the terms of the GNU
 * General Public License version 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.  Please review the following information to
 * ensure the GNU General Public License version 3.0 requirements will be
 * met: http://www.gnu.org/copyleft/gpl.html.
 */

#include "SignalType.h"

namespace Vector {
namespace DBC {

SignalType::SignalType() :
    name(),
    size(0),
    byteOrder(ByteOrder::BigEndian),
    valueType(ValueType::Unsigned),
    factor(0.0),
    offset(0.0),
    minimum(0.0),
    maximum(0.0),
    unit(),
    defaultValue(0.0),
    valueTable()
{
    /* nothing to do here */
}

}
}
