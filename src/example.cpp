#include <iostream>
#include <libssh/libssh.h>


int main(int argc, char**argv)
{
	char* b64_key[1];
	std::cout << "Hello world\n";
	auto k = ssh_key_new();
	if (k == NULL)
	{
		std::cout << "KEY 0x0" << std::endl;
		free(k);
		return 0;
	}

	auto key_generation_result = ssh_pki_generate(SSH_KEYTYPE_RSA, 4096, &k);


	if (key_generation_result == SSH_OK)
	{
		std::cout << "Key generation result SSH_OK" << std::endl;
		auto pub_k = ssh_key_new();
		if (pub_k == NULL)
		{
			std::cout << "PUB KEY 0x0" << std::endl;
			free(k);
			return 0;
		}
		else
		{
			auto pub_key_generation_result = ssh_pki_export_privkey_to_pubkey(k, &pub_k);
			ssh_pki_export_pubkey_base64(pub_k, b64_key);
			std::cout << "Pub Key:\n" << b64_key[0] << std::endl;
			free(b64_key[0]);
		}
		free(pub_k);
	}
	else
	{
		std::cout << "Key generation result SSH_ERROR" << std::endl;
	}

	free(k);
	return 0;

}



