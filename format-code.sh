#!/bin/bash

#
# Copyright (C) 2020-2021 Christoph Sommer <sommer@cms-labs.org>
#
# Documentation for these modules is at http://veins.car2x.org/
#
# SPDX-License-Identifier: GPL-2.0-or-later
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#

echo "WARNING: the format-code.sh script is deprecated in favor of bin/plexe_format_code. Redirecting." >&2
echo "WARNING: the format-code.sh script is deprecated in favor of bin/plexe_format_code. Redirecting."

$(dirname "$0")/bin/plexe_format_code "$@"

echo "WARNING: the format-code.sh script is deprecated in favor of bin/plexe_format_code. Redirection done." >&2
echo "WARNING: the format-code.sh script is deprecated in favor of bin/plexe_format_code. Redirection done."
