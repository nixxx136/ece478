/*
 *      Author: Jordan Bayles (baylesj), baylesj@engr.orst.edu
 *     Created: 04/28/14 10:27:27
 *    Filename: collision_finder.cpp
 *
 * Description: Finds truncated md5 collisions
 *
 *       Notes: - cookie content is <username>.<timestamp>.<mac>
 *              - data is <username>.<timestamp>
 *              - MAC is hex encoded hash of data hashed, truncated, and hashed
 *                with the key
 *              - mac stored in cookie must match MAC made from
 *                <username>.<timestamp>
 *              - 
 *
 */

#include <iostream>
#include <map>
#include <string>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/stdcpp.h>
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>

using namespace std;

struct data
{
string field;
string hash;
}

class CollisionFinder
{
public:
	CollisionFinder(string timestamp, int max, int truncate);
	string find_collision();

	static const string m_chars;
	static const string m_ints;

private:
	string m_timestamp;             // timestamp in future to use

	int    m_max_len;               // Max length of fields to generate
	int    m_truncate;              // truncation length for md5trunc

	CryptoPP::Weak::MD5 m_hash;     // cryptopp hash instance
	CryptoPP::HexEncoder m_encoder; // hex encoder for conversions

	vector<data> m_admin_hashes;    // hashes for admin acct
	vector<data> m_user_hashes;     // hashes for usernames acct
};

const string CollisionFinder::m_chars = "abcdefghijklmnopqrstuvwxyz";
const string CollisionFinder::m_ints = "0123456789";

CollisionFinder::CollisionFinder(string timestamp, int max, int truncate)
: m_timestamp(timestamp), m_max_len(max), m_truncate(truncate)
{
}

string CollisionFinder::find_collision()
{
	byte digest[CryptoPP::Weak::MD5::DIGESTSIZE];
	string output;

	for (int i = 0; i < m_max_len; ++i) {
		for (auto j = m_chars.begin(); j != m_chars.end(); ++j) {
			data entry;
			// calculate the hash
			m_hash.CalculateDigest(digest,
				       (byte *)input.c_str(),
					       input.length()
					       );

			// hex encode the results
			m_encoder.Attach(new CryptoPP::StringSink(entry.hash));
			m_encoder.Put(digest, sizeof(digest));
			m_encoder.MessageEnd();

			// truncate the hash!
			output.erase(m_truncate, string::npos);
		}

		for (auto j = m_ints.begin(); j != m_ints.end(); ++j) {

		}
	}
}

int main (int argc, char **argv)
{
	cout << "md5 collision finder" << endl;

	// Need to get parameters from user
	cout << "Timestamp to use?" << endl;
	string timestamp;
	cin >> timestamp;

	cout << "Truncate length to use?" << endl;
	int truncate;
	cin >> truncate;

	cout << "Max length string to try for collision?" << endl;
	int max;
	cin >> max;

	// run the collision
	CollisionFinder c(timestamp, max, truncate);
	string result = c.find_collision();

	if (!result.empty()) {
		cout << "Found collision with username: " << endl << result << endl;
	} else {
		cout << "Didn't find a collision. Larger max length?" << endl;
	}

	return 0;
}
