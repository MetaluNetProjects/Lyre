/*********************************************************************
 *
 *                HMC5883L 3-axis digital compass for Fraise
 *
 *********************************************************************
 * Author               Date        Comment
 *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Antoine Rousseau  sept 2015     Original.
 ********************************************************************/

/*
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA  02110-1301, USA.
*/

void HMC5883_init();

void HMC5883_service();

unsigned int HMC5883_getX();
unsigned int HMC5883_getZ();
unsigned int HMC5883_getY();

