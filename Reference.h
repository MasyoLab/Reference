//==========================================================================
// Reference [Reference.h]
// author : MasyoLab
//==========================================================================
#pragma once

#include <memory>

namespace reference
{
	//==========================================================================
	//
	// class : IReference 
	//
	// English
	// Content : This function is an extension of weak_ptr.
	//
	// Japanese
	// Content : weak_ptr ���g�������N���X�ł��B
	//
	//==========================================================================
	template <typename _Ty>
	class IReference :
		private std::weak_ptr<_Ty>
	{
	public:
		IReference() {}
		~IReference() {}
	private:
		// weak_ptr �̋@�\�� private �W�J���܂�
		using std::weak_ptr<_Ty>::expired;
		using std::weak_ptr<_Ty>::lock;
		using std::weak_ptr<_Ty>::reset;
	public:
		// weak_ptr �̋@�\�� public �W�J���܂�
		using std::weak_ptr<_Ty>::weak_ptr;
		using std::weak_ptr<_Ty>::use_count;
		using std::weak_ptr<_Ty>::operator=;
		using std::weak_ptr<_Ty>::element_type;

		/**
		@brief �Ď��Ώۂ̎����؂�⃊���N�؂�𔻒肷��
		@return �Ď��I�u�W�F�N�g�����݂���ꍇ�� true�A�����łȂ��ꍇ �� false
		*/
		bool check() const noexcept {
			return !expired();
		}
		// return pointer to resource
		_Ty* operator->() const noexcept {
			return lock().get();
		}
		// return reference to resource
		_Ty& operator*() const noexcept {
			return *lock().get();
		}
		/**
		@brief �Ď��Ώۂ��Ď�����̂���߂܂�
		*/
		void clear() {
			reset();
		}
		void operator =(nullptr_t) {
			reset();
		}
		operator bool() const noexcept {
			return !expired();
		}
		bool operator!=(nullptr_t) const noexcept {
			return !expired();
		}
		bool operator==(nullptr_t) const noexcept {
			return expired();
		}
		bool operator==(const IReference& _Right) const noexcept {
			if (expired())
				return false;
			if (_Right.expired())
				return false;
			return lock() == _Right.lock();
		}
		bool operator!=(const IReference& _Right) const noexcept {
			if (expired())
				return false;
			if (_Right.expired())
				return false;
			return lock() != _Right.lock();
		}
	};

	//==========================================================================
	//
	// class : WrapperReference 
	//
	// English
	// Content : Monitoring function reference class
	//
	// Japanese
	// Content : �Ď��@�\�̎Q�ƃN���X
	//
	//==========================================================================
	template <typename _Derived, typename _Base, bool isExtended = std::is_base_of<_Base, _Derived>::value>
	class WrapperReference
	{
		static_assert(isExtended, "WrapperReference <> : _Ty is not inherited from BaseClass Class");
	public:
		using Owned = _Derived;
	public:
		WrapperReference() {}
		WrapperReference(const std::shared_ptr<_Base>& _This) : m_weak(_This) {}
		WrapperReference(const IReference<_Base>& _This) : m_weak(_This) {}
		WrapperReference(const WrapperReference& _Right) : m_weak(_Right.m_weak) {}
		~WrapperReference() {}

		operator bool() const noexcept {
			return m_weak.check();
		}
		bool operator!=(nullptr_t) const noexcept {
			return m_weak != nullptr;
		}
		bool operator==(nullptr_t) const noexcept {
			return m_weak == nullptr;
		}
		bool operator==(const IReference<_Base>& _Right) const noexcept {
			if (!m_weak.check())
				return false;
			if (!_Right.check())
				return false;
			return m_weak == _Right;
		}
		bool operator!=(const IReference<_Base>& _Right) const noexcept {
			if (!m_weak.check())
				return false;
			if (!m_weak.check())
				return false;
			return m_weak != _Right;
		}
		template<class _Ty>
		bool operator!=(WrapperReference <_Ty, _Base>& _Right) const noexcept {
			if (!m_weak.check())
				return false;
			if (!m_weak.check())
				return false;
			return m_weak != _Right.m_weak;
		}
		template<class _Ty>
		bool operator==(WrapperReference <_Ty, _Base>& _Right) const noexcept {
			if (!m_weak.check())
				return false;
			if (!m_weak.check())
				return false;
			return m_weak == _Right.m_weak;
		}
		void operator =(nullptr_t) {
			m_weak.clear();
		}
		// return pointer to resource
		_Derived* operator->() const noexcept {
			return (_Derived*)m_weak.operator->();
		}
		// return reference to resource
		_Derived& operator*() const noexcept {
			return *(_Derived*)m_weak.operator->();
		}

		/**
		English
		@brief checks whether the referenced object was already deleted
		@return True if the managed object exists, false otherwise
		Japanese
		@brief �Ď��Ώۂ̎����؂�⃊���N�؂�𔻒肷��
		@return �Ď��I�u�W�F�N�g�����݂���ꍇ�� true�A�����łȂ��ꍇ �� false
		*/
		bool check() const noexcept {
			return m_weak.check();
		}

		/**
		English
		@brief returns the number of shared_ptr objects that manage the object
		@return The number of shared_ptr instances sharing the ownership of the managed object at the instant of the call.
		Japanese
		@brief �Ď����Ă���shared_ptr�I�u�W�F�N�g�̏��L�Ґ����擾����
		@return shared_ptr�Ăяo���̏u�ԂɊǗ��ΏۃI�u�W�F�N�g�̏��L�������L���Ă���C���X�^���X�̐��B
		*/
		long use_count() const noexcept {
			return m_weak.use_count();
		}
		/**
		@brief �Ď��Ώۂ��Ď�����̂���߂܂�
		*/
		void clear() {
			m_weak.clear();
		}
	protected:
		IReference<_Base> m_weak; // �Ď��@�\
	};
}
