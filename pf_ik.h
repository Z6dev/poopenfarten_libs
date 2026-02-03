/*
*   Things Related to 2d IK and stickman
*   Because I suck at animation :( but gud at coding :D
*	Heh, I've been getting into stickman animation community lately!
*
*	# USAGE #
*	=========
*
*	#define PF_IK_IMPLEMENTATION
*	in SPECIFICALLY ONLY ONE C/C++ FILE
*/

#ifndef PF_IK_H
#define PF_IK_H

#include <raylib.h>
#include <raymath.h>

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

// base Ik struct, combine it to make stickman
typedef struct {
	Vector2 root;  // shoulder/hip
	Vector2 joint; // elbow/knee
	Vector2 end;   // hand/foot

	float len1; // upper bone
	float len2; // lower bone
} IKChain;

//----------------------------------------------------------------------------------
// Stickman/IK Functions Declaration
//----------------------------------------------------------------------------------
void pfSolveIK(IKChain *chain, Vector2 target, int bendDir); // Update IK chain to move to specified target, bendDir >= 0 is right, bendDir < 0
void pfDrawIK(IKChain chain, float thick, Color color);		 // Draw IK chain using raylib

//----------------------------------------------------------------------------------
// Stickman/IK Functions Implementation
//----------------------------------------------------------------------------------
#ifdef PF_IK_IMPLEMENTATION

// Update IK chain to move to specified target
void pfSolveIK(IKChain *chain, Vector2 target, int bendDir) {
	Vector2 root = chain->root;

	// Vector from root to target
	Vector2 toTarget = Vector2Subtract(target, root);
	float dist = Vector2Length(toTarget);

	// Clamp distance so it can't stretch too far
	dist = Clamp(dist, 0.0001f, chain->len1 + chain->len2);

	// Angle from root to target
	float baseAngle = atan2f(toTarget.y, toTarget.x);

	// Whatever this "Law of cosine" thingy is
	float cosAngle1 =
		(chain->len1 * chain->len1 + dist * dist - chain->len2 * chain->len2) /
		(2.0f * chain->len1 * dist);

	cosAngle1 = Clamp(cosAngle1, -1.0f, 1.0f);

	float angle1 = acosf(cosAngle1);

	// Joint position (elbow/knee)
	bendDir = bendDir >= 0 ? 1 : -1;
	float jointAngle = baseAngle + bendDir * angle1;

	chain->joint.x = root.x + cosf(jointAngle) * chain->len1;
	chain->joint.y = root.y + sinf(jointAngle) * chain->len1;

	// End position (hand/foot)
	Vector2 jointToTarget = Vector2Subtract(target, chain->joint);
	float angle2 = atan2f(jointToTarget.y, jointToTarget.x);

	chain->end.x = chain->joint.x + cosf(angle2) * chain->len2;
	chain->end.y = chain->joint.y + sinf(angle2) * chain->len2;
}

// Draw IK chain using raylib
void pfDrawIK(IKChain chain, float thick, Color color) {
	DrawLineEx(chain.root, chain.joint, thick, color);
	DrawLineEx(chain.joint, chain.end, thick, color);
}

#endif // PF_IK_IMPLEMENTATION

#endif // PF_IK_H