#pragma once

namespace kra {
	using HandleT = int;
	class NetSaveBuffer;
	class NetLoadBuffer;

	template<typename T>
	class Handle {
	public:
		Handle();
		Handle(HandleT Handle);
		Handle(const Handle<T>& Other);
		Handle(Handle<T>&& Other);

		Handle<T>& operator=(const Handle<T>& Other);
		Handle<T>& operator=(Handle<T>&& Other);

		// Check
		bool operator==(const Handle<T>& Other);
		bool operator!=(const Handle<T>& Other);

		// Get the value of the handle
		HandleT GetHandle() const;

		// Set the value of the handle
		// This should only be called from a manager
		void SetHandle(HandleT Handle);

		// Check if the handle is valid
		bool IsValid() const;

		// Make the handle invalid
		void MakeInvalid();

	public: // Networking
		void NetSave(NetSaveBuffer& Buff);
		void NetLoad(NetLoadBuffer& Buff);

	private:
		HandleT HandleVal;
	};

	template<typename T>
	inline Handle<T>::Handle() 
		: HandleVal(-1)
	{
	}

	template<typename T>
	inline Handle<T>::Handle(HandleT Handle)
		: HandleVal(Handle)
	{
	}

	template<typename T>
	inline Handle<T>::Handle(const Handle<T>& Other)
		: HandleVal(Other.HandleVal)
	{
	}

	template<typename T>
	inline Handle<T>::Handle(Handle<T>&& Other)
		: HandleVal(Other.HandleVal)
	{
		Other.MakeInvalid();
	}

	template<typename T>
	inline Handle<T>& Handle<T>::operator=(const Handle<T>& Other)
	{
		HandleVal = Other.HandleVal;
		return *this;
	}

	template<typename T>
	inline Handle<T>& Handle<T>::operator=(Handle<T>&& Other)
	{
		HandleVal = Other.HandleVal;
		Other.MakeInvalid();
		return *this;
	}

	template<typename T>
	inline bool Handle<T>::operator==(const Handle<T>& Other)
	{
		return HandleVal == Other.HandleVal;
	}

	template<typename T>
	inline bool Handle<T>::operator!=(const Handle<T>& Other)
	{
		return HandleVal != Other.HandleVal;
	}

	template<typename T>
	inline HandleT Handle<T>::GetHandle() const
	{
		return HandleVal;
	}

	template<typename T>
	inline void Handle<T>::SetHandle(HandleT Handle)
	{
		HandleVal = Handle;
	}

	template<typename T>
	inline bool Handle<T>::IsValid() const
	{
		return HandleVal != -1;
	}

	template<typename T>
	inline void Handle<T>::MakeInvalid()
	{
		HandleVal = -1;
	}

	template<typename T>
	inline void Handle<T>::NetSave(NetSaveBuffer & Buff)
	{
		Buff << HandleVal;
	}

	template<typename T>
	inline void Handle<T>::NetLoad(NetLoadBuffer & Buff)
	{
		Buff >> HandleVal;
	}
}