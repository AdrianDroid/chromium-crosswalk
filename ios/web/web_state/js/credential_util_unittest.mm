// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ios/web/web_state/js/credential_util.h"

#include "base/memory/scoped_ptr.h"
#include "base/strings/utf_string_conversions.h"
#include "base/values.h"
#include "ios/web/public/web_state/credential.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "testing/gtest_mac.h"
#include "testing/platform_test.h"
#include "url/gurl.h"

namespace web {
namespace {

// "type" value for a DictionaryValue representation of LocalCredential.
const char* kTestCredentialTypeLocal = "LocalCredential";

// "type" value for a DictionaryValue representation of FederatedCredential.
const char* kTestCredentialTypeFederated = "FederatedCredential";

// "id" value for a DictionaryValue representation of a credential.
const char* kTestCredentialID = "foo";

// "name" value for a DictionaryValue representation of a credential.
const char* kTestCredentialName = "Foo Bar";

// "avatarURL" value for a DictionaryValue representation of a credential.
const char* kTestCredentialAvatarURL = "https://foo.com/bar.jpg";

// "password" value for a DictionaryValue representation of a credential.
const char* kTestCredentialPassword = "baz";

// "federationURL" value for a DictionaryValue representation of a credential.
const char* kTestCredentialFederationURL = "https://foo.com/";

// Determines whether two credentials are equal.
bool CredentialsEqual(const Credential& credential1,
                      const Credential& credential2) {
  return credential1.type == credential2.type &&
         credential1.id == credential2.id &&
         credential1.name == credential2.name &&
         credential1.avatar_url == credential2.avatar_url &&
         credential1.password == credential2.password &&
         credential1.federation_url == credential2.federation_url;
}

// Returns a Credential with Local type.
Credential GetTestLocalCredential() {
  Credential credential;
  credential.type = CredentialType::CREDENTIAL_TYPE_LOCAL;
  credential.id = base::ASCIIToUTF16(kTestCredentialID);
  credential.name = base::ASCIIToUTF16(kTestCredentialName);
  credential.avatar_url = GURL(kTestCredentialAvatarURL);
  credential.password = base::ASCIIToUTF16(kTestCredentialPassword);
  return credential;
}

// Returns a credential with Federated type.
Credential GetTestFederatedCredential() {
  Credential credential;
  credential.type = CredentialType::CREDENTIAL_TYPE_FEDERATED;
  credential.id = base::ASCIIToUTF16(kTestCredentialID);
  credential.name = base::ASCIIToUTF16(kTestCredentialName);
  credential.avatar_url = GURL(kTestCredentialAvatarURL);
  credential.federation_url = GURL(kTestCredentialFederationURL);
  return credential;
}

// Returns a value representing the credential returned by
// |GetTestLocalCredential()|.
scoped_ptr<base::DictionaryValue> GetTestLocalCredentialDictionaryValue() {
  scoped_ptr<base::DictionaryValue> value(new base::DictionaryValue);
  value->SetString("type", kTestCredentialTypeLocal);
  value->SetString("id", kTestCredentialID);
  value->SetString("name", kTestCredentialName);
  value->SetString("avatarURL", kTestCredentialAvatarURL);
  value->SetString("password", kTestCredentialPassword);
  return value.Pass();
}

// Returns a value representing the credential returned by
// |GetTestFederatedCredentialDictionaryValue()|.
scoped_ptr<base::DictionaryValue> GetTestFederatedCredentialDictionaryValue() {
  scoped_ptr<base::DictionaryValue> value(new base::DictionaryValue);
  value->SetString("type", kTestCredentialTypeFederated);
  value->SetString("id", kTestCredentialID);
  value->SetString("name", kTestCredentialName);
  value->SetString("avatarURL", kTestCredentialAvatarURL);
  value->SetString("federation", kTestCredentialFederationURL);
  return value.Pass();
}

// Tests that parsing an empty value fails.
TEST(CredentialUtilTest, ParsingEmptyValueFails) {
  base::DictionaryValue value;
  Credential credential;
  EXPECT_FALSE(DictionaryValueToCredential(value, &credential));
}

// Tests that parsing a value with a bad type fails.
TEST(CredentialUtilTest, ParsingValueWithBadTypeFails) {
  scoped_ptr<base::DictionaryValue> value(new base::DictionaryValue);
  value->SetString("type", "FooCredential");
  Credential credential;
  EXPECT_FALSE(DictionaryValueToCredential(*value, &credential));
}

// Tests that parsing a correctly-formed value representing a LocalCredential
// succeeds.
TEST(CredentialUtilTest, ParsingLocalCredentialSucceeds) {
  Credential credential;
  EXPECT_TRUE(DictionaryValueToCredential(
      *GetTestLocalCredentialDictionaryValue(), &credential));
  EXPECT_TRUE(CredentialsEqual(GetTestLocalCredential(), credential));
}

// Tests that parsing a value representing a LocalCredential but with no ID
// specified fails.
TEST(CredentialUtilTest, ParsingLocalCredentialWithNoIDFails) {
  scoped_ptr<base::DictionaryValue> value(
      GetTestLocalCredentialDictionaryValue().Pass());
  value->RemoveWithoutPathExpansion("id", nullptr);
  Credential credential;
  EXPECT_FALSE(DictionaryValueToCredential(*value, &credential));
}

// Tests that parsing a value representing a LocalCredential with a badly-
// formed avatarURL fails.
TEST(CredentialUtilTest, ParsingLocalCredentialWithBadAvatarURLFails) {
  scoped_ptr<base::DictionaryValue> value(
      GetTestLocalCredentialDictionaryValue().Pass());
  value->SetString("avatarURL", "foo");
  Credential credential;
  EXPECT_FALSE(DictionaryValueToCredential(*value, &credential));
}

// Tests that parsing a value representing a LocalCredential with no password
// specified fails.
TEST(CredentialUtilTest, ParsingLocalCredentialWithNoPasswordFails) {
  scoped_ptr<base::DictionaryValue> value(
      GetTestLocalCredentialDictionaryValue().Pass());
  value->Remove("password", nullptr);
  Credential credential;
  EXPECT_FALSE(DictionaryValueToCredential(*value, &credential));
}

// Tests that parsing a correctly-formed value representing a
// FederatedCredential succeeds.
TEST(CredentialUtilTest, ParsingFederatedCredentialSucceeds) {
  Credential credential;
  EXPECT_TRUE(DictionaryValueToCredential(
      *GetTestFederatedCredentialDictionaryValue(), &credential));
  EXPECT_TRUE(CredentialsEqual(GetTestFederatedCredential(), credential));
}

// Tests that parsing a value representing a FederatedCredential with no ID
// fails.
TEST(CredentialUtilTest, ParsingFederatedCredentialWithNoIDFails) {
  scoped_ptr<base::DictionaryValue> value(
      GetTestFederatedCredentialDictionaryValue().Pass());
  value->RemoveWithoutPathExpansion("id", nullptr);
  Credential credential;
  EXPECT_FALSE(DictionaryValueToCredential(*value, &credential));
}

// Tests that parsing a value representing a FederatedCredential with a badly-
// formed avatarURL fails.
TEST(CredentialUtilTest, ParsingFederatedCredentialWithBadAvatarURLFails) {
  scoped_ptr<base::DictionaryValue> value(
      GetTestFederatedCredentialDictionaryValue().Pass());
  value->SetString("avatarURL", "foo");
  Credential credential;
  EXPECT_FALSE(DictionaryValueToCredential(*value, &credential));
}

// Tests that parsing a value representing a FederatedCredential with no
// federation URL fails.
TEST(CredentialUtilTest, ParsingFederatedValueWithNoFederationURLFails) {
  scoped_ptr<base::DictionaryValue> value(
      GetTestFederatedCredentialDictionaryValue().Pass());
  value->Remove("federation", nullptr);
  Credential credential;
  EXPECT_FALSE(DictionaryValueToCredential(*value, &credential));
}

// Tests that parsing a value representing a FederatedCredential with a badly-
// formed federationURL fails.
TEST(CredentialUtilTest, ParsingFederatedValueWithBadFederationURLFails) {
  scoped_ptr<base::DictionaryValue> value(
      GetTestFederatedCredentialDictionaryValue().Pass());
  value->SetString("federation", "bar");
  Credential credential;
  EXPECT_FALSE(DictionaryValueToCredential(*value, &credential));
}

// Tests that serializing a FederatedCredential to a DictionaryValue results
// in the expected structure.
TEST(CredentialUtilTest, SerializeFederatedCredential) {
  base::DictionaryValue value;
  Credential credential(GetTestFederatedCredential());
  CredentialToDictionaryValue(credential, &value);
  EXPECT_TRUE(GetTestFederatedCredentialDictionaryValue()->Equals(&value));
}

// Tests that serializing a LocalCredential to a DictionaryValue results in the
// expected structure.
TEST(CredentialUtilTest, SerializeLocalCredential) {
  base::DictionaryValue value;
  Credential credential(GetTestLocalCredential());
  CredentialToDictionaryValue(credential, &value);
  EXPECT_TRUE(GetTestLocalCredentialDictionaryValue()->Equals(&value));
}

}  // namespace
}  // namespace web
