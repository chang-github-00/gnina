/*
 * PDBQTUtilities.h
 *
 *  Created on: Jun 4, 2014
 *      Author: dkoes
 */

/**********************************************************************
Copyright (C) 1998-2001 by OpenEye Scientific Software, Inc.
Some portions Copyright (C) 2003-2006 Geoffrey R. Hutchison
Some portions Copyright (C) 2004 by Chris Morley
Some portions Copyright (C) 2014 by David Koes and the University of Pittsburgh

Original Copyright refers to the pdbformat.cpp file, for reading and
writing pdb format files.
Extensively modified 2010 Stuart Armstrong (Source Science/InhibOx)
for the purpose of reading and writing pdbqt format files.
Some portions Copyright (C) 2010 by Stuart Armstrong of Source Science/
InhibOx

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
***********************************************************************/

#ifndef PDBQTUTILITIES_H_
#define PDBQTUTILITIES_H_

#include <openbabel/mol.h>
#include <iostream>
#include <set>
#include "parsing.h"

using namespace std;
using namespace OpenBabel;

class obbranch
{
public:
	vector<int> atoms;
	bool done;
	unsigned int index;
	set<unsigned int> children;
	vector<unsigned int> parents;
	unsigned int depth;
	unsigned int connecting_atom_parent;
	unsigned int connecting_atom_branch;
	unsigned int how_many_atoms_moved;

	set<unsigned int> rigid_with; //the other branches that move rigidly with this one

	void clear()
	{
		done = false;
		index = 0;
		depth = 0;
		connecting_atom_parent = 0;
		connecting_atom_branch = 0;
		how_many_atoms_moved = 0;
		children.clear();
		parents.clear();
		atoms.clear();
		rigid_with.clear();
		parents.push_back(0);
	}
	unsigned int UpOne()
	{
		if (parents.size() >= 2)
		{
			return parents.at(parents.size() - 2);
		}
		return 0;
	}
	obbranch()
	{
		clear();
	}
	void all_atoms(OBMol& mol)
	{
		clear();
		rigid_with.insert(0);
		for (unsigned int i = 1; i <= mol.NumAtoms(); i++)
		{
			atoms.push_back(i);
		}
	}
};

bool IsRotBond_PDBQT(OBBond * the_bond);
bool IsIn(const vector<int>& vec, const int num);

unsigned int AtomsSoFar(const map<unsigned int, obbranch>& tree,
		unsigned int depth);
bool FindBondedPiece(const vector<int>& root, const vector<int>& branch,
		unsigned int& root_atom, unsigned int& branch_atom,
		unsigned int& root_atom_rank, unsigned int& branch_atom_rank,
		const OBMol& mol, unsigned int & atoms_moved);
bool OutputTree(OBMol& mol, context& lines, parsing_struct& p,
		map<unsigned int, obbranch>& tree, unsigned int depth);
void ConstructTree(map<unsigned int, obbranch>& tree,
		vector<vector<int> > rigid_fragments, unsigned int root_piece,
		const OBMol& mol, bool flexible);
bool DeleteHydrogens(OBMol & mol);
bool Separate_preserve_charges(OBMol & mol, vector<OBMol> & result);
unsigned int FindFragments(OBMol mol,
		vector<vector<int> >& rigid_fragments);
unsigned int RotBond_count(OBMol & mol);

#endif /* PDBQTUTILITIES_H_ */
