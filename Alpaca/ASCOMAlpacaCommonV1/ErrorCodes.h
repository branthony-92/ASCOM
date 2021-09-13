#ifndef ERRORCODES_H
#define ERRORCODES_H

namespace Alpaca {
namespace ErrorCodes
{
	const int c_actionNotImplimented      = 0x8004040C;
	const int c_driverBase			      = 0x80040500;
	const int c_invalidOperationException = 0x8004040B; 
	const int c_invalidValue              = 0x80040401;
	const int c_invalidWhileParked        = 0x80040408;
	const int c_invalidWhileSlaved        = 0x80040409;
	const int c_notConnected              = 0x80040407;
	const int c_notImplemented            = 0x80040400;
	const int c_notInCacheException       = 0x8004040D;
	const int c_settingsProviderError     = 0x8004040A;
	const int c_unspecifiedError          = 0x800404FF;
	const int c_valueNotSet               = 0x80040402; 
	const int c_driverMax                 = 0x80040FFF;
};
}
#endif // !ERRORCODES_H