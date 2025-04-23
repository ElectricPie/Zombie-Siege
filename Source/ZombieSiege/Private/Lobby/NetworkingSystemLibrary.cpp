// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/NetworkingSystemLibrary.h"

bool UNetworkingSystemLibrary::IsValidIPAddress(const FString& IpAddress)
{
	const FRegexPattern Pattern(TEXT("^(25[0-5]|2[0-4][0-9]|1?[0-9]{1,2})(\\.(25[0-5]|2[0-4][0-9]|1?[0-9]{1,2})){3}$"));
	FRegexMatcher Matcher(Pattern, IpAddress);
	return Matcher.FindNext();
}
