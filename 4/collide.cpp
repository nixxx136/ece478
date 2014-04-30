/*
 *      Author:  Jordan Bayles (baylesj), baylesj@engr.orst.edu
 *     Created:  04/28/14 10:27:27
 *    Filename:  collision_finder.cpp
 *
 * Description:  Finds truncated md5 collisions
 */

#include <iostream>
#include <map>
#include <string>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/stdcpp.h>
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>

using namespace std;

class CollisionFinder
{
public:
	CollisionFinder(string post, string collide, int max);
	string find_collision();

	static const string m_valid_chars;
private:

	map<string, string> m_precalc;  // saved hash results

	string m_postfix;               // rest of input string (timestamp)
	string m_collide_hash;          // Hash to collide with

	int    m_max_len;               // Max length of username to try
	int    m_truncate;              // truncation length for md5trunc

	CryptoPP::Weak::MD5 m_hash;     // cryptopp hash instance
	CryptoPP::HexEncoder m_encoder; // hex encoder for conversions
};

const string CollisionFinder::m_valid_chars = "abcdefghijklmnopqrstuvwxyz";

CollisionFinder::CollisionFinder(string post, string collide, int max)
: m_postfix(post), m_collide_hash(collide), m_max_len(max)
{
	m_truncate = m_collide_hash.length();
}

string CollisionFinder::find_collision()
{
	string username;
	string input = username + m_postfix;
	string output;

	byte digest[CryptoPP::Weak::MD5::DIGESTSIZE];

	for (int i = 0; i < m_max_len; ++i) {
		for (auto j = m_valid_chars.begin(); j != m_valid_chars.end(); ++j) {

			// calculate the hash
#ifdef MULTIPLE_RUNS
			if (m_precalc.find(username) == m_precalc.end()) {
#endif
				m_hash.CalculateDigest(digest,
				               (byte *)input.c_str(),
				                       input.length()
				                       );

				// hex encode the results
				m_encoder.Attach(new CryptoPP::StringSink(output));
				m_encoder.Put(digest, sizeof(digest));
				m_encoder.MessageEnd();

				// truncate the hash!
				output.erase(m_truncate, string::npos);

#ifdef MULTIPLE_RUNS
			} else {
				output = m_precalc.find(username);
			}
#endif

			// breakout if we find what we wanted
			if (output == m_collide_hash) {
				return output;
			}
		}
	}

	output.clear();
	return output;
}

int main (int argc, char **argv)
{
	cout << "md5 collision finder" << endl;

	// Need to get parameters from user
	cout << "Desired collision?" << endl;
	string collision;
	cin >> collision;

	cout << "Timestamp to use?" << endl;
	string timestamp;
	cin >> timestamp;

	cout << "Max length string to try for collision?" << endl;
	int max;
	cin >> max;


	// run the collision
	CollisionFinder c(timestamp, collision, max);
	string result = c.find_collision();

	if (!result.empty()) {
		cout << "Found collision with username: " << endl << result << endl;
	} else {
		cout << "Didn't find a collision. Larger max length?" << endl;
	}

	return 0;
}
